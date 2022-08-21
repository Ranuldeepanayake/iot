#include "DHT.h"

#define LDR_PIN 32
#define DHT_PIN 5

#define DHTTYPE DHT22

// variable for storing the potentiometer value
float potValue = 0;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Reading potentiometer value
  potValue = map(analogRead(LDR_PIN), 0, 4095, 0, 100);
  Serial.println(potValue);
  delay(500);
}
