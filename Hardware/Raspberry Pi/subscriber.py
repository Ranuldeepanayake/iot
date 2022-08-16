# python3.8
import random

from paho.mqtt import client as mqtt_client

#Network information
broker = 'a417d22d.ap-southeast-1.emqx.cloud'
port = 15136

# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 1000)}'
username = 'ranul'
password = 'iot5120'

#Data
topic = "humidity"


def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

    client.subscribe(topic)
    client.on_message = on_message


if __name__ == '__main__':
    while 1:
        client = connect_mqtt()
        subscribe(client)
        client.loop_forever()