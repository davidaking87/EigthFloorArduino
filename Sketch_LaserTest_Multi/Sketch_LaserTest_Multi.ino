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

#include "Adafruit_VL6180X.h"

Adafruit_VL6180X lasers;
  byte laser1SHDNPin = 7; // SHutDowN pin
  byte laser2SHDNPin = 6;
//  byte laser3SHDNPin = 3;

void setup() {
  pinMode(laser1SHDNPin, OUTPUT);
  pinMode(laser2SHDNPin, OUTPUT);
//  pinMode(laser3SHDNPin, OUTPUT);
  digitalWrite(laser1SHDNPin, LOW); // Disable laser1
  digitalWrite(laser2SHDNPin, LOW); // Disable laser2
//  digitalWrite(laser3SHDNPin, HIGH); // Disable laser

  Serial.begin(9600);
}

void loop() {
  // Enable laser 1
  digitalWrite(laser2SHDNPin, LOW);  // Disable laser2 before...
  digitalWrite(laser1SHDNPin, HIGH); // enabling laser1
  // Print laser 1 value
  Serial.print("laser1: ");
  lasers.begin();
  Serial.print(lasers.readRange());

  // Enable laser 2
  digitalWrite(laser1SHDNPin, LOW);  // Disable laser1 before...
  digitalWrite(laser2SHDNPin, HIGH); // enabling laser2
  // Print laser 2 value
  Serial.print("\t\tlaser2: ");
  lasers.begin();
  Serial.println(lasers.readRange());
  delay(500);
}
