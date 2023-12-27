import paho.mqtt.publish as publish
import base64
MQTT_HOST = "localhost"
MQTT_PORT = 1883

# To be implemented depending on the type of connection used
class Connection:
    def __init__(self):
        pass

    def write(self, data):
        print(f"Writing data: {data}")

        publish.single("sender/packet", payload=base64.b64encode(data), qos=0, 
        hostname=MQTT_HOST, port=MQTT_PORT)
