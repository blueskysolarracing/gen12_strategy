import struct
from shared.utilities import calculate_crc
from shared.protocol_ids import Sender_ID, Chase_Data_ID

class BssrProtocolSender:

    BSSR_SERIAL_START = 0xa5
    BSSR_SERIAL_ESCAPE = 0x5a
    PAD = True

    # Packet constants
    START_BYTE = 165
    NUM_CRC_BYTES = 0x4

    def __init__(self, connection):
        self.connection = connection
        self.seq_num = 0

    def send_serial(self, payload):        
        def escape_byte(byte_val, packet):
            if byte_val in [self.BSSR_SERIAL_START, self.BSSR_SERIAL_ESCAPE]:
                packet.append(self.BSSR_SERIAL_ESCAPE)
                packet.append(byte_val)
            else:
                packet.append(byte_val)

        # Initializing packet with Start Byte, Payload Length, Sender ID, and Sequence Number
        packet = [self.BSSR_SERIAL_START, len(payload), Sender_ID.CHASE_ID, self.seq_num]

        # Add payload to packet without escape functionality (for CRC calculation)
        packet += payload

        # Pad the data to a multiple of 4 (if defined and required)
        # Note: Assuming PAD is a defined variable (True/False) in the class or module
        if self.PAD and (len(packet) % 4) != 0:
            print("Padding packet")
            padding_num = 4 - (len(packet) % 4)
            packet += [0x00] * padding_num

        # Calculate CRC and append to the packet
        CRC_bytes = calculate_crc(packet, len(payload), use_numpy=False)
        packet += CRC_bytes

        print("raw packet:  ", bytes(packet).hex())

        # Escape necessary fields and values
        escaped_packet = []
        escaped_packet.append(self.START_BYTE)
        escape_byte(len(payload), escaped_packet)  # Payload Length
        escaped_packet.append(Sender_ID.CHASE_ID)
        escape_byte(self.seq_num, escaped_packet)  # Sequence Number

        # Add the payload with escape functionality
        for byte_val in payload:
            escape_byte(byte_val, escaped_packet)

        # Add CRC values with escape functionality
        for crc_byte in CRC_bytes:
            escape_byte(crc_byte, escaped_packet)

        # Increment the sequence number
        self.seq_num += 1

        print("Sent packet: ", bytes(escaped_packet).hex())
        print("Packet length: ", len(escaped_packet))
        self.connection.write(bytearray(escaped_packet))
        
    def _vfm_sender(self, vmf_state):
        payload = [Chase_Data_ID.CHASE_VMF_ID, vmf_state, 0x00, 0x00]
        self.send_serial(payload)

    def _eco_sender(self, eco_on):
        payload = [Chase_Data_ID.CHASE_ECO_MODE_ID, eco_on, 0x00, 0x00]
        self.send_serial(payload)

    def _cruise_PI_sender(self, k_p: float, k_i: float, k_d: float):
        payload = [
            Chase_Data_ID.CHASE_CRUISE_PI_GAIN_ID, int(k_p != None), int(k_i != None), int(k_d != None),
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        ]
        GAIN_SCALE_FACTOR = 100000

        if k_p != None:
            k_p = int(GAIN_SCALE_FACTOR * k_p)
            byte_array = list(struct.pack('I', k_p))
            for i in range(4):
                payload[4+i] = byte_array[3-i]

        if k_i != None:
            k_i = int(GAIN_SCALE_FACTOR * k_i)
            byte_array = list(struct.pack('I', k_i))
            for i in range(4):
                payload[8+i] = byte_array[3-i]

        if k_d != None:
            k_d = int(GAIN_SCALE_FACTOR * k_d)
            byte_array = list(struct.pack('I', k_d))
            for i in range(4):
                payload[12+i] = byte_array[3-i]
        
        self.send_serial(payload)

    def _fault_enable_sender(self, fault_enable):
        payload = [Chase_Data_ID.CHASE_FAULT_ENABLE_ID, fault_enable, 0x00, 0x00]
        self.send_serial(payload)

    def phrase_sender(self, phrase):
        print(f"Sending Message: {phrase}")
        MAX_PHRASE_LENGTH = 11

        if len(phrase) < MAX_PHRASE_LENGTH:
            new_phrase = phrase + ' ' * (MAX_PHRASE_LENGTH -len(phrase))
        else: 
            new_phrase = phrase
        
        payload = bytes(new_phrase, 'utf-8').hex()
        # turn payload into list of bytes
        payload = [Chase_Data_ID.CHASE_MESSAGE_ID] + [int(payload[i:i+2], 16) for i in range(0, len(payload), 2)]
        print(f"ID and word encoding: {payload}")
        self.send_serial(payload)

    def vfm_up_sender(self):
        print("VFM Up")
        self._vfm_sender(0x01)

    def vfm_down_sender(self):
        print("VFM Down")
        self._vfm_sender(0x00)

    def eco_on_sender(self):
        print("ECO On")
        self._eco_sender(0x01)
    
    def eco_off_sender(self):
        print("ECO Off")
        self._eco_sender(0x00)

    def cruise_PI_KP_sender(self, k_p):
        self._cruise_PI_sender(k_p, None, None)
    
    def cruise_PI_KI_sender(self, k_i):
        self._cruise_PI_sender(None, k_i, None)

    def cruise_PI_KD_sender(self, k_d):
        self._cruise_PI_sender(None, None, k_d)

    def f_enable_sender(self):
        self._fault_enable_sender(0x01)

    def f_disable_sender(self):
        self._fault_enable_sender(0x00)