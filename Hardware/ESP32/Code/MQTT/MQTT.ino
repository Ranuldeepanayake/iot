/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define LED_BUILT_IN 2
#define LED_RED 26
#define LED_GREEN 27
#define LDR_PIN 32

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

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT
  //*********

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(LED_BUILT_IN, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(LED_BUILT_IN, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      
      // Wait 3 seconds before retrying
      delay(2000);
    }
  }
}

void setup() {
  //Health check LED blink.
  pinMode(LED_BUILT_IN, OUTPUT);
  digitalWrite(LED_BUILT_IN, HIGH);
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  Serial.begin(115200);

  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  digitalWrite(LED_BUILT_IN, LOW);
}

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
    airTemperature = bmp.readTemperature();   
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
    ambientLight = map(analogRead(LDR_PIN), 0, 4095, 0, 100);
    //Convert the value to a char array
    char lightString[8];
    dtostrf(ambientLight, 1, 2, lightString);
    Serial.print("Ambient light: "); Serial.println(lightString);
    client.publish("agrismart/ambientLight", lightString);
    digitalWrite(LED_GREEN, LOW);
  }
}
