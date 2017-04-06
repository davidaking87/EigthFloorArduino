// Site: http://arduino.stackexchange.com/questions/25050/multiple-vl6180x-distance-sensors-interactive-problem

#include "Adafruit_VL6180X.h"

Adafruit_VL6180X laser1;
  byte laser1SHDNPin = 1; // SHutDowN pin
  #define LASER1_ADDRESS 0x30;
Adafruit_VL6180X laser2;
  byte laser2SHDNPin = 2;
Adafruit_VL6180X laser3;
  byte laser3SHDNPin = 3;

void setup() {
  pinMode(laser1SHDNPin, OUTPUT);
//  pinMode(laser2SHDNPin, OUTPUT);
//  pinMode(laser3SHDNPin, OUTPUT);
  digitalWrite(laser1SHDNPin, HIGH); // Disable laser
//  digitalWrite(laser2SHDNPin, HIGH); // Disable laser
//  digitalWrite(laser3SHDNPin, HIGH); // Disable laser
  
  digitalWrite(laser1Pin, LOW);
  laser1.begin();
  laser1.setAddress(LASER1_ADDRESS);
  
  laser2.begin();
  laser3.begin();
  sensor.begin();
  Serial.begin(9600);
}

void loop() {
//  Serial.println(sensor.readReg(0x212),HEX);
  Serial.println(sensor.readRange());
  delay(500);
}
