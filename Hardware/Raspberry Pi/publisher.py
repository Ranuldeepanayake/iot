# python 3.8
import random
import time

from paho.mqtt import client as mqtt_client

# #Network information
# broker = 'a417d22d.ap-southeast-1.emqx.cloud'
# port = 15136

# # generate client ID with pub prefix randomly
# client_id = f'python-mqtt-{random.randint(0, 1000)}'
# username = 'ranul'
# password = 'iot5120'

#Network information
broker = '192.168.1.29'
port = 1883

# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 1000)}'
username = 'ranul'
password = 'iot5120'

#Data
topic = "temperature"
message= 97

#Create a connection to the MQTT broker
def connect_mqtt():
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

#Publish a topic to the MQTT broker
def publish(client):
    result = client.publish(topic, message)
    # result: [0, 1]
    status = result[0]
    if status == 0:
        print(f"Sent `{message}` to topic `{topic}`")
    else:
        print(f"Failed to send message to topic {topic}")
    
# #This is the main program loop. Do sensor readings and actuator control here
# if __name__ == '__main__':
#     while 1:  
#         client = connect_mqtt()  
#         publish(client)
#         print("I am running in the background")
#         time.sleep(2)
        
#This is the main program loop. Do sensor readings and actuator control here
if __name__ == '__main__':
    client = connect_mqtt()  
    while 1:  
        publish(client)
        print("I am running in the background")
        time.sleep(2)