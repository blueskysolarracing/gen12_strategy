import numpy as np
import time
import struct
import base64
import multiprocessing as mp
import serial
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import paho.mqtt.subscribe as subscribe
import json
import os
import sys
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, BASE_DIR)
import shared.utilities as utilities
from shared.protocol_ids import Chase_Data_ID



#SERIAL_PORT = '/dev/pts/3'
SERIAL_PORT = '/dev/ttyUSB0'

MQTT_HOST = "localhost"
MQTT_PORT = 1883

BMS = "BMS"
MCMB = "MCMB"
DCMB = "DCMB"
BBMB = "BBMB"
PPTMB = "PPTMB"
MAX_PAYLOAD_SIZE = 24
MAX_PACKET_SIZE = 32
ESCAPED_BYTE = 90
STARTING_BYTE = 165
NUM_CRC_BYTES = 4

# Sender ID's
BBMB_SENDER_ID = 1
PPTMB_SENDER_ID = 2
MCMB_SENDER_ID = 3
DCMB_SENDER_ID = 4
# chase and bms are not currently being used

# Data ID's
BUS_METRICS = 0
CAR_SPEED = 1
MOTOR_TEMPERATURE = 2
MOTOR_CONTROL = 5
RELAY_STATE = 6
CELL_TEMPERATURE = 7
CELL_VOLTAGE = 8
CELL_SOC = 9
BMS_HEARTBEAT = 10
LIGHT_CONTROL = 3
SIDE_PANEL = 8
CONTROLS = 4
HEARTBEAT = 15

ERROR_WINDOW = 5 # seconds





class Parser:
    def __init__(self, byte_buffer):
        self.byte_buffer = byte_buffer
        self.client = mqtt.Client()
        self.client.connect(MQTT_HOST, MQTT_PORT, 60)
        # Indicate the status of each byte in a packet
        self.escaped_received = False
        self.started_received = False
        self.length_received = False
        self.sender_received = False
        self.sequence_received = False
        self.data_id_received = False
        self.payload_received = False
        self.crc_received = False
        self.packet_complete = False
        self.packet_corrupt = False

        # Packet values
        self.sender = -1
        self.payload_length = -1
        self.data_id = -1
        self.payload = np.zeros(MAX_PAYLOAD_SIZE, dtype=np.uint8)
        self.packet = np.zeros(MAX_PACKET_SIZE, dtype=np.uint8)
        self.packets_processed = 0
        self.crc = np.zeros(NUM_CRC_BYTES, dtype=np.uint8)
        self.sequence_bytes = {
            1: -1, # BBMB
            2: -1, # PPTMB
            3: -1, # MCMB
            4: -1, # DCMB
            5: -1, # Not Used
            6: -1, # Not Used
        }
        self.allBytes = []
        self.valid_crc = False

        # State Variables  
        self.packet_index = 0
        self.packet_length = 0

        # Radio health
        self.num_packets_received = 0
        self.num_packets_corrupted = 0

        self.reference_time = time.time()


        # MCMB
        self.mcmb_car_speed = 0

    def run(self):
        """
        Read from the queue of bytes
        """    
        count = 0
        while True:
            count += 1
            byte = self.byte_buffer.get()[0]
           # Escaped byte 0x5a - 90 
            if byte == ESCAPED_BYTE and self.escaped_received != True:
                self.escaped_received = True
            # Starting byte
            elif byte == STARTING_BYTE and self.started_received != True:
                self.started_received = True
                self.packet[self.packet_index] = byte
                self.packet_index += 1
            # length byte
            elif self.started_received and self.length_received != True:
                if self.escaped_received:
                    self.escaped_received = False
            
                self.length_received = True
                self.payload_length = byte

                self.packet[self.packet_index] = byte
                self.packet_index += 1
                # If the payload length exceeds the maximum size, drop the byte and start the packet again
                if (self.payload_length > 24):
                    self.reset_loop_variables()
                
            # sender byte (board)
            elif self.started_received and self.length_received and self.sender_received != True:
                if self.escaped_received:
                    self.escaped_received = False
                self.sender_received = True
                self.sender = byte

                self.packet[self.packet_index] = byte
                self.packet_index += 1
                # If the sender byte is invalid, drop the byte and start again
                if (self.sender < 1 or self.sender > 6):
                    self.reset_loop_variables()
            # sequence byte
            elif self.started_received and self.length_received and self.sender_received and self.sequence_received != True:
                if self.escaped_received:
                    self.escaped_received = False
                
                self.sequence_received = True
                self.packet[self.packet_index] = byte

                last_sequence_byte = self.sequence_bytes[self.sender]
                dropped_packets = byte - last_sequence_byte if last_sequence_byte != -1 and byte - last_sequence_byte > 1 else 0
                self.num_packets_corrupted += dropped_packets 
                self.sequence_bytes[self.sender] = byte
                self.packet_index += 1
            # data id byte - also needs to go in payload array
            elif self.started_received and self.length_received and self.sender_received and self.sequence_received and self.data_id_received != True:
                if self.escaped_received:
                    self.escaped_received = False

                self.data_id_received = True
                self.data_id = byte
                self.packet[self.packet_index] = byte
                self.payload[self.payload_length - 1] = byte
                self.packet_index += 1

                # If the data id goes past 15, drop the byte and start again
                if (self.data_id > 15):
                    self.reset_loop_variables()

            # payload bytes
            elif self.started_received and self.length_received and self.sender_received and self.sequence_received and self.data_id_received and self.payload_received != True:
                idx = 1

                while idx < self.payload_length:
                    # escaped byte
                    if byte == ESCAPED_BYTE and self.escaped_received != True:
                        self.escaped_received = True
                        byte = self.byte_buffer.get()[0]
                        continue # don't increment the index properties
                    elif self.escaped_received:
                        self.escaped_received = False

                    self.packet[self.packet_index] = byte

                    # We store it in reverse order to swap the endians
                    self.payload[self.payload_length - idx - 1] = byte

                    idx += 1
                    self.packet_index += 1

                    if idx < self.payload_length:
                        byte = self.byte_buffer.get()[0]
                
                self.payload_received = True
            
            # crc bytes
            elif self.started_received and self.length_received and self.sender_received and self.sequence_received and self.data_id_received and self.payload_received and self.crc_received != True:
                idx = 0

                while idx < NUM_CRC_BYTES:
                    # Escaped byte
                    if byte == ESCAPED_BYTE and self.escaped_received != True:
                        self.escaped_received = True
                        byte = self.byte_buffer.get()[0]
                        continue # don't increment the index properties
                    elif self.escaped_received:
                        self.escaped_received = False

                    self.packet[self.packet_index] = byte
                    self.crc[idx] = byte
                    idx += 1
                    self.packet_index += 1   

                    if idx < NUM_CRC_BYTES:
                        byte = self.byte_buffer.get()[0]

                self.packet_complete = True

            if self.packet_complete:
                # process crc
                calculated_crc = utilities.calculate_crc(self.packet, self.payload_length)
                crc_bytes = self.packet[self.packet_index-4:self.packet_index]
                if np.array_equal(calculated_crc, crc_bytes):
                    self.valid_crc = True
                    self.num_packets_received += 1
                    self.extract_data()
                else:
                    self.num_packets_corrupted += 1
                    print("crc dropped")
                    print(calculated_crc)
                    print(crc_bytes)

                self.reset_loop_variables()

            current_time = time.time()
            if (current_time - self.reference_time > ERROR_WINDOW):
                self.reference_time = current_time
                # Reset sender variables
                self.sequence_bytes = {
                    1: -1, # BBMB
                    2: -1, # PPTMB
                    3: -1, # MCMB
                    4: -1, # DCMB
                    5: -1, # Not Used
                    6: -1, # Not Used
                }
                self.num_packets_corrupted = 0
                self.num_packets_received = 0 

    def reset_loop_variables(self):
        """
        Reset loop variables in order to construct the next packet
        """
        self.escaped_received = False
        self.started_received = False
        self.length_received = False
        self.sender_received = False
        self.sequence_received = False
        self.data_id_received = False
        self.payload_received = False
        self.crc_received = False
        self.packet_complete = False
        self.packet_corrupt = False
        self.names_to_change = []
        self.values_to_change = []

        self.sender = -1
        self.payload_length = -1
        self.data_id = -1
        self.payload = np.zeros(MAX_PAYLOAD_SIZE, dtype=np.uint8)
        self.packet = np.zeros(MAX_PACKET_SIZE, dtype=np.uint8)
        self.crc = np.zeros(NUM_CRC_BYTES, dtype=np.uint8)
        self.packet_index = 0
        self.valid_crc = False

    def extract_data(self):
        """
        Parse the packet stored in self.packet and update the gui
        print("got data")
        """      
        # BBMB
        if self.sender == BBMB_SENDER_ID:
            if self.data_id == HEARTBEAT:
                self.send_heartbeat(BBMB, self.payload[2])

            elif self.data_id == BUS_METRICS:
                self.send_bus_metrics(BBMB, struct.unpack('f', self.payload[0:4])[0], 
                                      struct.unpack('f', self.payload[4:8])[0])
            elif self.data_id == BMS_HEARTBEAT:
                self.send_heartbeat(BMS, self.payload[2])
            elif self.data_id == CELL_TEMPERATURE:
                self.send_cell_temps()
            elif self.data_id == CELL_VOLTAGE:
                self.send_cell_volts()
            elif self.data_id == CELL_SOC:
                self.send_cell_soc()
            elif self.data_id == RELAY_STATE:
                pass
            elif self.data_id == Chase_Data_ID.CHASE_FAULT_ENABLE_ID:
                print("fault enable ack")
                if self.payload[2] == 1:
                    print("fault enabled")
                else:
                    print("fault disabled")

        # PPTMB
        elif self.sender == PPTMB_SENDER_ID:
            if self.data_id == HEARTBEAT:
                self.send_heartbeat(PPTMB, self.payload[2])

            elif self.data_id == BUS_METRICS:
                self.send_bus_metrics(PPTMB, struct.unpack('f', self.payload[0:4])[0], struct.unpack('f', self.payload[4:8])[0])

        
        # MCMB
        elif self.sender == MCMB_SENDER_ID:
            if self.data_id == HEARTBEAT:
                self.send_heartbeat(MCMB, self.payload[2])
            elif self.data_id == CAR_SPEED:
                self.client.publish("mcmb/car_speed", json.dumps({"car_speed":int(self.payload[2])}))
                #self.client.publish("mcmb/car_speed", json.dumps({"car_speed":struct.unpack('u', self.payload[1:4])}))
            elif self.data_id == MOTOR_TEMPERATURE:
                self.client.publish("mcmb/motor_temp", json.dumps({"motor_temp":struct.unpack('f', self.payload[0:4])[0]}))

            elif self.data_id == BUS_METRICS:
                self.send_bus_metrics(MCMB, struct.unpack('f', self.payload[0:4])[0], struct.unpack('f', self.payload[4:8])[0])
            elif self.data_id == Chase_Data_ID.CHASE_CRUISE_PI_GAIN_ID:
                print("received cruise control ack")
                k_p = struct.unpack('I', self.payload[8:12])[0] / 100000
                k_i = struct.unpack('I', self.payload[4:8])[0] / 100000
                k_d = struct.unpack('I', self.payload[0:4])[0] / 100000
                if self.payload[14] == 1:
                    print(f"k_p set to {k_p}")
                if self.payload[13] == 1:
                    print(f"k_i set to {k_i}")
                if self.payload[12] == 1:
                    print(f"k_d set to {k_d}")
        # DCMB
        elif self.sender == DCMB_SENDER_ID:
            if self.data_id == HEARTBEAT:
                self.send_heartbeat(DCMB, self.payload[2])
            elif self.data_id == MOTOR_CONTROL:
                target_power = struct.unpack('<H', self.payload[6:8])[0]

                publish.multiple([
                    ("dcmb/vfm_position", json.dumps({"vfm_position":int(self.payload[8])}), 0, False),
                    ("dcmb/target_power", json.dumps({"target_power":float(target_power)}), 0, False),
                    ("dcmb/target_speed", json.dumps({"target_speed":int(self.payload[3])}), 0, False),
                    ("dcmb/motor_state", json.dumps({"motor_state": int(self.payload[10])}), 0, False)
                ], hostname=MQTT_HOST, port=MQTT_PORT, client_id="", keepalive=60)
            elif self.data_id == LIGHT_CONTROL:
                left_right_indicator = (self.payload[2] & 0b00000001)
                indicator = (self.payload[2] & 0b00000010) >> 1
                drl = (self.payload[2] & 0b00000100) >> 2
                brake_lights = (self.payload[2] & 0b00001000) >> 3
                hazard_lights = (self.payload[2] & 0b00010000) >> 4
                print(bin(self.payload[2]))
                publish.multiple([
                    ("dcmb/lights/indicator", json.dumps({"indicator": int(indicator)}), 0, False),
                    ("dcmb/lights/left_right", json.dumps({"left_right": int(left_right_indicator)}), 0, False),
                    ("dcmb/lights/drl", json.dumps({"drl": int(drl)}), 0, False),
                    ("dcmb/lights/brake_lights", json.dumps({"brake_lights": int(brake_lights)}), 0, False),
                    ("dcmb/lights/hazard_lights", json.dumps({"hazard_lights": int(hazard_lights)}), 0, False)
                ], hostname=MQTT_HOST, port=MQTT_PORT, client_id="", keepalive=60)


    def send_heartbeat(self, name, value):
        self.client.publish("heartbeats/" + name, json.dumps({name:int(value)}))
                
    def send_bus_metrics(self, name, current, voltage):
        publish.multiple([
            ("bus_metrics/current/" + name, json.dumps({name: current}), 0, False),
            ("bus_metrics/voltage/" + name, json.dumps({name: voltage}), 0, False)
        ], hostname=MQTT_HOST, port=MQTT_PORT, client_id="", keepalive=60,)
    
    def send_cell_temps(self):
        module_id = self.payload[14]
        topic = "cell_metrics/temperature/module/" + str(module_id) + "/cell/"
        packets = []
        for i in range(3):
            value = struct.unpack('f', self.payload[(2-i)*4:(3-i)*4])[0]
            # TODO send temp fault
            if value != -1000:
                packets.append((topic + str(i), 
                    json.dumps(
                        {"module_{}_cell_{}".format(module_id, i): value}),
                        0,
                    False))
        publish.multiple(packets, hostname=MQTT_HOST, port=MQTT_PORT, client_id="", keepalive=60,)

    def send_cell_data(self, topic_name):
        module_id = self.payload[22]
        topic = "cell_metrics/{}/module/{}/cell/".format(topic_name, module_id)
        packets = []
        for i in range(5):
            value = struct.unpack('f', self.payload[(4-i)*4:(5-i)*4])[0]
            if value != -1000:
                packets.append((topic + str(i),
                            json.dumps(
                                {"module_{}_cell_{}".format(module_id, i): value}),
                                0,
                                False))
        publish.multiple(packets, hostname=MQTT_HOST, port=MQTT_PORT, client_id="", keepalive=60,)

    def send_cell_volts(self):
        self.send_cell_data("voltage")
        pass

    def send_cell_soc(self):
        self.send_cell_data("soc")

def start_parser(byte_buffer: mp.Queue):
    """Start the parser"""
    parser = Parser(byte_buffer)
    parser.run()

def read_serial(byte_buffer: mp.Queue, send_buffer: mp.Queue):
    """Read from serial and write to buffer"""
    ser = serial.Serial(SERIAL_PORT, 115200)
    while True:
        #try:
        # TODO safe exit for shutdown with try finally
        byte = ser.read(1)
        byte_buffer.put(byte)
        if not send_buffer.empty():
            data = send_buffer.get_nowait()
            ser.write(data)
        #finally:
        #    continue

def recieve_mqtt(sender_buffer):
    while 1:
        data = subscribe.simple("sender/packet", qos=0, msg_count=1,
            hostname=MQTT_HOST, 
            port=MQTT_PORT, keepalive=60)
        sender_buffer.put(base64.b64decode(data.payload))

def parser_task():
    byte_buffer = mp.Queue()
    send_buffer = mp.Queue()
    serial_proc = mp.Process(target=read_serial, args=(byte_buffer, send_buffer,))
    serial_proc.start()
    serial_parser = mp.Process(target=start_parser, args=(byte_buffer,))
    serial_parser.start()

    serial_sender = mp.Process(target=recieve_mqtt, args=(send_buffer,))
    serial_sender.start()



def main():
    """
    Run the parser
    """
    byte_buffer = mp.Queue()
    send_buffer = mp.Queue()
    serial_proc = mp.Process(target=read_serial, args=(byte_buffer, send_buffer,))
    serial_proc.start()
    serial_parser = mp.Process(target=start_parser, args=(byte_buffer,))
    serial_parser.start()

    serial_sender = mp.Process(target=recieve_mqtt, args=(send_buffer,))
    serial_sender.start()



if __name__ == "__main__":
    main()
