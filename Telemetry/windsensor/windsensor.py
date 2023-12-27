import serial
import paho.mqtt.publish as publish
import json

ser = serial.Serial('/dev/ttyUSB1', 9600)
import time

while True:
    time.sleep(0.01)
    data = ser.readline()
    data = data.decode('utf-8')
    new_data = data.split(',')
    if len(new_data) > 2:
        publish.multiple([
            ("wind_sensor/direction", 
            json.dumps({"direction": int(new_data[1])}), 0, False),
            ("wind_sensor/speed",
            json.dumps({"speed": float(new_data[2])}), 0, False)
        ], hostname="localhost", port=1883, client_id="", keepalive=60)

ser.close()
