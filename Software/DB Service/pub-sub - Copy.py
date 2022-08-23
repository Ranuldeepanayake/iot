# python 3.8
import random
import time

from paho.mqtt import client as mqtt_client

#Network information
broker = '192.168.1.29'
port = 1883

#Generate client ID with pub prefix randomly
#client_id = f'python-mqtt-{random.randint(0, 1000)}'
client_id = "database-service"
username = 'ranul'
password = 'iot5120'

#Topics to subscribe and push data to the database.
topicAmbientLight= "agrismart/ambientLight"
topicSoilMoisture= "agrismart/soilMoisture"
topicAirTemperature= "agrismart/airTemperature"

#Topics to publish data by reading the database.
topicTest= "agrismart/test"
topicLedStrip= "agrismart/ledStrip"
topicWaterPump= "agrismart/waterPump"
messageTest= "on"
messageLedStrip= 5
messageWaterPump= 0

#Create a connection to the MQTT broker
def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    #client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


#Publish a topic to the MQTT broker
def publish(client, topic, message):
    result= client.publish(topic, message)

    # result: [0, 1]
    status= result[0]

    if status == 0:
        print(f"Sent `{message}` to topic `{topic}`")
    else:
        print(f"Failed to send message to topic {topic}")


#Subscribe to a topic from the MQTT broker
def subscribe(client: mqtt_client, topic):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        #print("I am running in the background and below is the payload")
        print(msg.payload.decode())

        #Push data from the sensors to the database here.

    client.subscribe(topic)
    client.on_message = on_message
    

    
#This is the main program loop. Do sensor readings and actuator control here
if __name__ == '__main__':
    
    client= connect_mqtt()

    while 1:  
        #client= connect_mqtt()
        client.loop_start()

        print("Getting sensor data>")
        subscribe(client, topicAmbientLight)
        subscribe(client, topicSoilMoisture)
        subscribe(client, topicAirTemperature)
        print("\n")
        
        #Read the output table from the database continously and publish it to the MQTT server.
        print("Publishing actuator data>")
        publish(client, topicTest, messageTest)
        publish(client, topicLedStrip, messageLedStrip)
        publish(client, topicWaterPump, messageWaterPump)
        print("\n")
        
        #client.loop_forever()
        time.sleep(1)
        #client.disconnect()
        


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