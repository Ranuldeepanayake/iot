#include <Wire.h>
#include <Adafruit_BMP280.h>
//#include <Adafruit_Sensor.h>

Adafruit_BMP280 bmp; // I2C
float temperature = 0;

void setup() {

  //Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");

  //bmp.begin();
   if (!bmp.begin(0x76)) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

}

void loop() {
  temperature= bmp.readTemperature();
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  Serial.print("Temperature: ");
  Serial.println(tempString);
  
  //bmp.readPressure()
}
