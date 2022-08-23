# python 3.8
import random
import time

from paho.mqtt import client as mqtt_client

#Network information
broker = 'a417d22d.ap-southeast-1.emqx.cloud'
port = 15136

# generate client ID with pub prefix randomly
#client_id = f'python-mqtt-{random.randint(0, 1000)}'
client_id = "pub-sub-app"
username = 'ranul'
password = 'iot5120'

#Data
topic_1= "temperature"
topic_2= "humidity"
message_1= 97
message_2= 0

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
    result = client.publish(topic_1, message_1)
    # result: [0, 1]
    status = result[0]
    if status == 0:
        print(f"Sent `{message_1}` to topic `{topic_1}`")
    else:
        print(f"Failed to send message to topic {topic_1}")

#Subscribe to a topic from the MQTT broker
def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        #print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        print("I am running in the background and below is the payload")
        print(msg.payload.decode())

    client.subscribe(topic_2)
    client.on_message = on_message
    
    
    
#This is the main program loop. Do sensor readings and actuator control here
if __name__ == '__main__':
    
    while 1:  
        client = connect_mqtt()
        client.loop_start()

        publish(client)
        subscribe(client)

        #Do stuff
        # print("I am running in the background and below is the payload")
        # print(message_2)
        #message_2=
        
        #client.loop_forever()
        time.sleep(1)
        client.disconnect()
        


###############

# def connect_mqtt() -> mqtt_client:
#     def on_connect(client, userdata, flags, rc):
#         if rc == 0:
#             print("Connected to MQTT Broker!")
#         else:
#             print("Failed to connect, return code %d\n", rc)

#     client = mqtt_client.Client(client_id)
#     client.username_pw_set(username, password)
#     client.on_connect = on_connect
#     client.connect(broker, port)
#     return client


# if __name__ == '__main__':
#     while 1:
#         client = connect_mqtt()
#         subscribe(client)
#         client.loop_forever()