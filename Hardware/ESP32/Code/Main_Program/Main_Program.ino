/*********
 * This is the micro-controller code for the ESP32 v1 board we have used in our
 * M.Sc IoT project.
 * Uses the MQTT protocol to send and receive data through Wi-Fi. 
 * 
 * Sensors: BMP280, BH1750 and soil mositure sensor.
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
#include <BH1750.h>

//Status LEDs
#define LED_BUILT_IN 2
#define LED_RED 26
#define LED_GREEN 27

//Sensors
#define SOIL_MOISTURE_SENSOR_PIN 32
#define BATTERY_VOLTAGE_SENSOR_PIN 34

//LED strip 
#define LED_STRIP_PIN   33
#define LED_STRIP_PWM_CHANNEL    0
#define LED_STRIP_PWM_RESOLUTION   3
#define LED_STRIP_PWM_FREQUENCY  5000
#define LED_STRIP_INPUT_LOW 0
#define LED_STRIP_INPUT_HIGH 7

//Water pump 
#define WATER_PUMP_PIN   25
#define WATER_PUMP_PWM_CHANNEL    2
#define WATER_PUMP_PWM_RESOLUTION   3
#define WATER_PUMP_PWM_FREQUENCY  5000
#define WATER_PUMP_INPUT_LOW 0
#define WATER_PUMP_INPUT_HIGH 7

//Objects.
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_BMP280 bmp; //Uses I2C.
BH1750 lightMeter; //Uses I2C.

//Wi-Fi credentials.
const char* ssid = "RANGER HOME DIALOG";
const char* password = "rangerhomedialog";

//Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.1.29";

long lastMsg= 0;
char msg[50];
int value= 0;

//Sensor variables
float ambientLight= 0; //Float is ok
float soilMoisture= 0;
float airTemperature= 0; //Float is ok
float batteryVoltage= 0; 

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
  if (String(topic) == "agrismart/testLed") {
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
      Serial.print("Invalid LED control message!: ");
      Serial.println(value);
  }else{
      Serial.print("Changing output of the LED strip to: ");
      Serial.println(value);
      ledcWrite(LED_STRIP_PWM_CHANNEL, value);
    }
  }

  //Water pump control.
  if(String(topic) == "agrismart/waterPump") {
    uint16_t value= uint16_t(messageTemp.toInt());
    
    if(value< WATER_PUMP_INPUT_LOW || value> WATER_PUMP_INPUT_HIGH){
      Serial.print("Invalid water pump control message!: ");
      Serial.println(value);
  }else{
      Serial.print("Changing output of the water pump to: ");
      Serial.println(value);
      ledcWrite(WATER_PUMP_PWM_CHANNEL, value);
    }
  }
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
      client.subscribe("agrismart/testLed");
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

  //Status LED setup.
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  //DAC setup.
  //For the LED strip.
  ledcAttachPin(LED_STRIP_PIN, LED_STRIP_PWM_CHANNEL);
  ledcSetup(LED_STRIP_PWM_CHANNEL, LED_STRIP_PWM_FREQUENCY, LED_STRIP_PWM_RESOLUTION);
  ledcWrite(LED_STRIP_PWM_CHANNEL, 0);

  //For the water pump.
  ledcAttachPin(WATER_PUMP_PIN, WATER_PUMP_PWM_CHANNEL);
  ledcSetup(WATER_PUMP_PWM_CHANNEL, WATER_PUMP_PWM_FREQUENCY, WATER_PUMP_PWM_RESOLUTION);
  ledcWrite(WATER_PUMP_PWM_CHANNEL, 0);

  //Serial setup.
  Serial.begin(115200);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();

  //BMP280 setup.
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  //BH1750 setup.
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);

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
  if (now - lastMsg > 1500) {
    lastMsg = now;

    ////
    //Read temperature in Celsius
    digitalWrite(LED_GREEN, HIGH);
    airTemperature= bmp.readTemperature();   
    //Convert the value to a char array
    char tempString[10];
    dtostrf(airTemperature, 1, 2, tempString);
    Serial.print("Temperature: "); Serial.println(tempString);
    client.publish("agrismart/airTemperature", tempString);
    digitalWrite(LED_GREEN, LOW);
    ////

    ////
    //Read the soil moisture as a percentage.
    soilMoisture= 100- (map(analogRead(SOIL_MOISTURE_SENSOR_PIN), 0, 4095, 0, 100));
    // Convert the value to a char array
    char soilString[10];
    dtostrf(soilMoisture, 1, 2, soilString);
    Serial.print("Soil moisture: ");
    Serial.println(soilString);
    client.publish("agrismart/soilMoisture", soilString);
    digitalWrite(LED_GREEN, LOW);
    ////

    ////
    //Read the ambient light as a LUX value.
    digitalWrite(LED_GREEN, HIGH);
    ambientLight= lightMeter.readLightLevel();
    //Convert the value to a char array
    char lightString[10];
    dtostrf(ambientLight, 1, 2, lightString);
    Serial.print("Ambient light: "); Serial.println(lightString);
    client.publish("agrismart/ambientLight", lightString);
    digitalWrite(LED_GREEN, LOW);
    ////

    //Read the internal battery voltage as a percentage.
    digitalWrite(LED_GREEN, HIGH);
    batteryVoltage= map(analogRead(BATTERY_VOLTAGE_SENSOR_PIN), 0, 4095, 0, 100);
    //batteryVoltage= map(batteryVoltage, 3, 4, 0, 100);
    //Convert the value to a char array
    char batteryString[20];
    dtostrf(batteryVoltage, 1, 2, batteryString);
    Serial.print("Battery level: "); Serial.println(batteryString);
    client.publish("agrismart/batteryLevel", batteryString);
    digitalWrite(LED_GREEN, LOW);
    ////
  }
}
