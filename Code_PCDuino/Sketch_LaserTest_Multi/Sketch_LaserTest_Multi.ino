// inspiration from site: http://arduino.stackexchange.com/questions/25050/multiple-vl6180x-distance-sensors-interactive-problem

/* Multiple laser procedure:
 *  1. Wire SHDN (SHutDowN) pins.
 *  2. Disable ALL lasers. (write SHDN pin > LOW)
 *  3. Enable 1 laser. (write SHDN pin > HIGH & call laser.begin())
 *  4. Read value.
 */
/* Note: 
 *  For Arduino Uno, SDA = A5 & SCL = A4
 *      for other boards, see: https://www.arduino.cc/en/Reference/Wire
 */
extern "C" {
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <inttypes.h>
}
#include "Adafruit_VL6180X.h"


Adafruit_VL6180X lasers;
  byte frontLaserSHDNPin = 1; // SHutDowN pin
  byte leftLaserSHDNPin = 3;
  byte rightLaserSHDNPin = 2;

int temp_front;
int temp_right;
int temp_left;

void setup() {
  printf("starting");
  pinMode(frontLaserSHDNPin, OUTPUT);
  pinMode(leftLaserSHDNPin, OUTPUT);
  pinMode(rightLaserSHDNPin, OUTPUT);
  digitalWrite(frontLaserSHDNPin, LOW); // Disable frontLaser
  digitalWrite(leftLaserSHDNPin, LOW); // Disable leftLaser
  digitalWrite(rightLaserSHDNPin, LOW); // Disable rightLaser
  
  Wire.begin(); // For lasers
}

// Adds 3ms delay
void readSensors() {
  // Enable left laser
  digitalWrite(leftLaserSHDNPin, HIGH); // enabling leftLaser
  // Print left laser value
  lasers.begin();
  delay(1);
  temp_left = lasers.readRange();
  printf("\tleftLaser: %3d    ", temp_left);
  digitalWrite(leftLaserSHDNPin, LOW);  // Disable leftLaser
//  delay(500);
  
  //printf("looping\n");
  // Enable front laser
  digitalWrite(frontLaserSHDNPin, HIGH); // enabling frontLaser
  // Print front laser value
  lasers.begin(); // Notice: every time laser is enabled, have to call begin()
  delay(1);
  temp_front = lasers.readRange();
  printf("frontLaser: %3d    ", temp_front);
  digitalWrite(frontLaserSHDNPin, LOW);  // Disable frontLaser
//  delay(500);
  
  // Enable right laser
  digitalWrite(rightLaserSHDNPin, HIGH); // enabling rightLaser
  // Print right laser value
  lasers.begin();
  delay(1);
  temp_right = lasers.readRange();
  printf("rightLaser: %3d\n", lasers.readRange());
  digitalWrite(rightLaserSHDNPin, LOW);  // Disable rightLaser
  //delay(500);
}

void loop() {
  readSensors();
  
  delay(50);
}
