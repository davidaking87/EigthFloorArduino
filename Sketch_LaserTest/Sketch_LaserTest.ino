// Site: http://arduino.stackexchange.com/questions/25050/multiple-vl6180x-distance-sensors-interactive-problem

#include "Adafruit_VL6180X.h"

Adafruit_VL6180X sensor;

void setup() {
  sensor.begin();
  Serial.begin(9600);
}

void loop() {
//  Serial.println(sensor.readReg(0x212),HEX);
  Serial.println(sensor.readRange());
  delay(500);
}
