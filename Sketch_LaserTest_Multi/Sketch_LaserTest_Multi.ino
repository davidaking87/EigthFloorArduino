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
  byte leftLaserSHDNPin = 2;
  byte rightLaserSHDNPin = 3;

void setup() {
  printf("starting");
  pinMode(frontLaserSHDNPin, OUTPUT);
  pinMode(leftLaserSHDNPin, OUTPUT);
  pinMode(rightLaserSHDNPin, OUTPUT);
  digitalWrite(frontLaserSHDNPin, LOW); // Disable frontLaser
  digitalWrite(leftLaserSHDNPin, LOW); // Disable leftLaser
  digitalWrite(rightLaserSHDNPin, LOW); // Disable rightLaser
  
  Wire.begin();
}

void loop() {
  printf("looping\n");
  // Enable laser 1
  digitalWrite(frontLaserSHDNPin, HIGH); // enabling frontLaser
  // Print laser 1 value
  lasers.begin(); // Notice: every time laser is enabled, have to call begin()
  delay(1);
  printf("\tfrontLaser: %d\n", lasers.readRange());
  digitalWrite(frontLaserSHDNPin, LOW);  // Disable frontLaser
//  delay(500);

  // Enable laser 2
  digitalWrite(leftLaserSHDNPin, HIGH); // enabling leftLaser
  // Print laser 2 value
  lasers.begin();
  delay(1);
  printf("\tleftLaser: %d\n", lasers.readRange());
  digitalWrite(leftLaserSHDNPin, LOW);  // Disable leftLaser
//  delay(500);
  
  // Enable laser 3
  digitalWrite(rightLaserSHDNPin, HIGH); // enabling rightLaser
  // Print laser 3 value
  lasers.begin();
  delay(1);
  printf("\trightLaser: %d\n", lasers.readRange());
  digitalWrite(rightLaserSHDNPin, LOW);  // Disable rightLaser
  delay(500);
}
