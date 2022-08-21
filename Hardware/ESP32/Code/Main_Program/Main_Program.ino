/*********
 * This is the micro-controller code for the ESP32 v1 board we have used in our
 * M.Sc IoT project.
 * Uses the MQTT protocol to send and receive data through Wi-Fi. 
 * 
 * Sensor: BMP280, LDR and soil mositure sensor.
 * Actuators: LED strip and micro-water pump.
 * 
 * Authors: Ranul Deepanayake, Rui Santos (https://randomnerdtutorials.com)
 * 
*********/

//Library includes
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

//Status LEDs
#define LED_BUILT_IN 2
#define LED_RED 26
#define LED_GREEN 27

//Sensors
#define LDR_PIN 32
//#define MOISTURE_SENSOR_PIN 34
#define BATTERY_VOLTAGE_PIN 34

//LED strip 
#define LED_STRIP_PIN   5
#define LED_STRIP_PWM_CHANNEL    0
#define LED_STRIP_PWM_RESOLUTION   3
#define LED_STRIP_PWM_FREQUENCY  5000
#define LED_STRIP_INPUT_LOW 0
#define LED_STRIP_INPUT_HIGH 7

//Micro-water pump 
//#define LED_STRIP_PIN   5
//#define LED_STRIP_PWM_CHANNEL    0
//#define LED_STRIP_PWM_RESOLUTION   3
//#define LED_STRIP_PWM_FREQUENCY  5000

//Objects.
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_BMP280 bmp; //Uses I2C.

//Wi-Fi credentials.
const char* ssid = "RANGER HOME DIALOG";
const char* password = "rangerhomedialog";

//Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.1.29";

long lastMsg= 0;
char msg[50];
int value= 0;

//Sensor variables
float airTemperature= 0;
int soilMoisture= 0;
int ambientLight= 0;
int batteryVoltage= 0; 

//Actuator variables
int ledStripPWM= 0;
int waterPumpPWM= 0;

//Wi-Fi connection setup.
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Callback function when a subscribed topic receives an update.
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

  //uint8_t msg= message[0];  
  //Concatenate the byte array into a string.
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  //Actuator control according to subscribed commands.
  
  //If a message is received on the topic 'agrismart/test', you check if the message is either "on" or "off". 
  //Changes the output state according to the message
  if (String(topic) == "agrismart/test") {
    Serial.print("Changing output of the test LED to: ");
    if(messageTemp == "on"){
      Serial.println(messageTemp);
      digitalWrite(LED_BUILT_IN, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println(messageTemp);
      digitalWrite(LED_BUILT_IN, LOW);
    }
  }

  //LED strip control.
  if(String(topic) == "agrismart/ledStrip") {
    uint16_t value= uint16_t(messageTemp.toInt());
    
    if(value< LED_STRIP_INPUT_LOW || value> LED_STRIP_INPUT_HIGH){
      Serial.print("Invalid message!: ");
      Serial.println(value);
  }else{
      Serial.print("Changing output of the LED strip to: ");
      Serial.println(value);
      ledcWrite(LED_STRIP_PWM_CHANNEL, value);
    }
  }

//  //Water pump control.
//  if(String(topic) == "agrismart/waterPump") {
//    if(messageTemp.toInt()< LED_STRIP_INPUT_LOW || messageTemp.toInt()> LED_STRIP_INPUT_HIGH){
//      Serial.print("Invalid message!: ");
//      Serial.println(messageTemp.toInt());
//  }else{
//      Serial.print("Changing output of the LED strip to: ");
//      Serial.println(messageTemp.toInt());
//      ledcWrite(PWM1_Ch, messageTemp.toInt());
//    }
//  }
}

//Reconnects with the MQTT server.
void reconnect() {
  
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      
      //Subscribe to topics.
      client.subscribe("agrismart/test");
      client.subscribe("agrismart/ledStrip");
      client.subscribe("agrismart/waterPump");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

//Initial setup code.
void setup() {
  //Health check LED blink.
  pinMode(LED_BUILT_IN, OUTPUT);
  digitalWrite(LED_BUILT_IN, HIGH);
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  //DAC setup.
  ledcAttachPin(LED_STRIP_PIN, LED_STRIP_PWM_CHANNEL);
  ledcSetup(LED_STRIP_PWM_CHANNEL, LED_STRIP_PWM_FREQUENCY, LED_STRIP_PWM_RESOLUTION);
  ledcWrite(LED_STRIP_PWM_CHANNEL, 0);

  //Serial setup.
  Serial.begin(115200);

  //BMP280 setup.
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  //Wi-Fi and MQTT setup.
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  digitalWrite(LED_BUILT_IN, LOW);
}

//Main program.
void loop() {
  //Establish a connection with the MQTT broker.
  if (!client.connected()) {
    digitalWrite(LED_RED, HIGH);
    reconnect();
  }
  digitalWrite(LED_RED, LOW);
  
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    digitalWrite(LED_GREEN, HIGH);
    //Read temperature in Celsius
    airTemperature= bmp.readTemperature();   
    //Convert the value to a char array
    char tempString[8];
    dtostrf(airTemperature, 1, 2, tempString);
    Serial.print("Temperature: "); Serial.println(tempString);
    client.publish("agrismart/airTemperature", tempString);
    digitalWrite(LED_GREEN, LOW);

  //Read the soil moisture as a percentage
//  humidity = bmp.readPressure();
//  // Convert the value to a char array
//  char humString[20];
//  dtostrf(humidity, 1, 2, humString);
//  Serial.print("Humidity: ");
//  Serial.println(humString);
//  client.publish("agrismart/humidity", humString);

    digitalWrite(LED_GREEN, HIGH);
    //Read the ambient light as a percentage
    ambientLight= map(analogRead(LDR_PIN), 0, 4095, 0, 100);
    //Convert the value to a char array
    char lightString[8];
    dtostrf(ambientLight, 1, 2, lightString);
    Serial.print("Ambient light: "); Serial.println(lightString);
    client.publish("agrismart/ambientLight", lightString);
    digitalWrite(LED_GREEN, LOW);

    digitalWrite(LED_GREEN, HIGH);
    //Read the internal battery voltage as a percentage
    batteryVoltage= map(analogRead(BATTERY_VOLTAGE_PIN), 0, 4095, 3.0, 4.2);
    batteryVoltage= map(batteryVoltage, 3.0, 4.2, 0, 100);
    //Convert the value to a char array
    char batteryString[8];
    dtostrf(batteryVoltage, 1, 2, batteryString);
    Serial.print("Battery level: "); Serial.println(batteryString);
    client.publish("agrismart/batteryLevel", batteryString);
    digitalWrite(LED_GREEN, LOW);
  }
}
