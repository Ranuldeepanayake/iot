# python 3.8
import random
import time
import mysql.connector

#MQTT library
from paho.mqtt import client as mqtt_client

#Database connection object
dbConnectionString= {
  "host" : "54.169.116.74",
  "user": "sep",
  "password" : "RHFkwLSa62uXb7vQ",
  "database" : "iot"
}

#mycursor= mydb.cursor()

#Network information
broker = '192.168.1.29'
port = 1883

#Generate client ID with pub prefix randomly
#client_id = f'python-mqtt-{random.randint(0, 1000)}'
client_id = "databaseService1"
username = 'ranul'
password = 'iot5120'

#Topics to subscribe and push data to the database.
topicAmbientLight= "agrismart/ambientLight"
topicSoilMoisture= "agrismart/soilMoisture"
topicAirTemperature= "agrismart/airTemperature"
messageAmbientLight= 0.0
messageSoilMoisture= 0.0
messageAirTemperature= 0.0

#Topics to publish data by reading the database.
topicTest= "agrismart/testLed"
topicLedStrip= "agrismart/ledStrip"
topicWaterPump= "agrismart/waterPump"
messageTest= "on"
messageLedStrip= 0
messageWaterPump= 0


def dbUpdate(messageAmbientLight, messageSoilMoisture, messageAirTemperature):
    print("Connecting to the database")
    sql= "update device set topic_ambient_light= %s, topic_soil_moisture= %s, topic_air_temperature= %s where device_id= 1"
    val= [messageAmbientLight, messageSoilMoisture, messageAirTemperature]
    
    mydb= mysql.connector.connect(**dbConnectionString)
    mycursor= mydb.cursor()
    mycursor.execute(sql, val)
    print(mycursor.rowcount, "Record updated")

    mydb.commit()
    mycursor.close()
    mydb.close()

def dbTopicAmbientLight(value):
    print("Connecting to the database")
    sql = "update device set topic_ambient_light= %s where device_id= 1"
    val = [value]
    
    mydb= mysql.connector.connect(**dbConnectionString)
    mycursor= mydb.cursor()
    mycursor.execute(sql, val)
    print(mycursor.rowcount, "Record updated")

    mydb.commit()
    mycursor.close()
    mydb.close()


def dbTopicSoilMoisture(value):
    print("Connecting to the database")
    sql = "update device set topic_soil_moisture= %s where device_id= 1"
    val = [value]
    
    mydb= mysql.connector.connect(**dbConnectionString)
    mycursor= mydb.cursor()
    mycursor.execute(sql, val)
    print(mycursor.rowcount, "Record updated")

    mydb.commit()
    mycursor.close()
    mydb.close()


def dbTopicAirTemperature(value):
    print("Connecting to the database")
    sql = "update device set topic_air_temperature= %s where device_id= 1"
    val = [value]
    
    mydb= mysql.connector.connect(**dbConnectionString)
    mycursor= mydb.cursor()
    mycursor.execute(sql, val)
    print(mycursor.rowcount, "Record updated")

    mydb.commit()
    mycursor.close()
    mydb.close()


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
        #print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        #print("I am running in the background and below is the payload")
        #print(msg.payload.decode())

        #Push data from the sensors to the database here.
        #DB code was removed from the callback to reduce the chances of the callback getting corrupted.

        if(msg.topic== topicAmbientLight):
            #dbTopicAmbientLight(msg.payload.decode())
            global messageAmbientLight
            messageAmbientLight= msg.payload.decode()

        elif(msg.topic== topicSoilMoisture):
            #dbTopicSoilMoisture(msg.payload.decode())
            global messageSoilMoisture
            messageSoilMoisture= msg.payload.decode()

        elif(msg.topic== topicAirTemperature):
            #dbTopicAirTemperature(msg.payload.decode())
            global messageAirTemperature
            messageAirTemperature= msg.payload.decode()
        
        
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

        #Push data to the database.
        
        # mycursor= mydb.cursor()
        dbUpdate(messageAmbientLight, messageSoilMoisture, messageAirTemperature)
        # dbTopicAmbientLight(messageAmbientLight)
        # dbTopicSoilMoisture(messageSoilMoisture)
        # dbTopicAirTemperature(messageAirTemperature)
        # mycursor.close()
        # mydb.close()
        
        #Read the output table from the database continously and publish it to the MQTT server.
        # print("Publishing actuator data>")
        # publish(client, topicTest, messageTest)
        # publish(client, topicLedStrip, messageLedStrip)
        # publish(client, topicWaterPump, messageWaterPump)
        # print("\n")
        
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