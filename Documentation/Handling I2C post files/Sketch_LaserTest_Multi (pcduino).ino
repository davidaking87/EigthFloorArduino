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
  byte laser1SHDNPin = 7; // SHutDowN pin
  byte laser2SHDNPin = 6;

void setup() {
  printf("starting");
  pinMode(laser1SHDNPin, OUTPUT);
  pinMode(laser2SHDNPin, OUTPUT);
  digitalWrite(laser1SHDNPin, LOW); // Disable laser1
  digitalWrite(laser2SHDNPin, LOW); // Disable laser2
  
  Wire.begin();
  /*
   // Doesn't work
  digitalWrite(laser2SHDNPin, LOW);  // Disable laser2 before...
  digitalWrite(laser1SHDNPin, HIGH); // enabling laser1
  /*
  lasers.begin();
  digitalWrite(laser1SHDNPin, LOW); // enabling laser1
  digitalWrite(laser2SHDNPin, HIGH);  // Disable laser2 before...
  lasers.begin();
  */
}

void loop() {
  printf("\tlooping");
  // Enable laser 1
  digitalWrite(laser2SHDNPin, LOW);  // Disable laser2 before...
  digitalWrite(laser1SHDNPin, HIGH); // enabling laser1
  // Print laser 1 value
  lasers.begin(); // Notice: every time laser is enabled, have to call begin()
  delay(1);
  printf("laser1: %d\n", lasers.readRange());
  delay(500);

  // Enable laser 2
  digitalWrite(laser1SHDNPin, LOW);  // Disable laser1 before...
  digitalWrite(laser2SHDNPin, HIGH); // enabling laser2
  // Print laser 2 value
  lasers.begin();
  delay(1);
  printf("\tlaser2: %d\n", lasers.readRange());
  delay(500);
}
