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

//void readSensors();

///////////////////////////////////////////////

Adafruit_VL6180X lasers;
  byte frontLaserSHDNPin = 1; // SHutDowN pin
  byte leftLaserSHDNPin = 3;
  byte rightLaserSHDNPin = 2;

int leftIPin1 = 12;
int leftIPin2 = 13;
int leftSpeedPin = 10; // enable motor A
boolean leftInvert = false;

int rightIPin1 = 8;
int rightIPin2 = 11;
int rightSpeedPin = 9; // Enable motor B
boolean rightInvert = true;

int SPEED = 50;
int correction = 25;
int temp_front;
  int prev_front;
int temp_right;
  int prev_right;
int temp_left;
  int prev_left;

int INTERSECTION_CHANGE = 100;
// Going really slowly, so delay = 5 seconds
int MIN_INTERSECT_DELAY = 5000; // Min time to wait between intersections
int timeSinceIntersect = MIN_INTERSECT_DELAY;

///////////////////////////////////////////////

void delayUpdateTimers(int time) { // Update timers
  delay(time);
  if (timeSinceIntersect < MIN_INTERSECT_DELAY) // Stop int overflows
    timeSinceIntersect += time;
}

///////////////////////////////////////////////

void setup() {
  printf("starting");
  pinMode(frontLaserSHDNPin, OUTPUT);
  pinMode(leftLaserSHDNPin, OUTPUT);
  pinMode(rightLaserSHDNPin, OUTPUT);
  digitalWrite(frontLaserSHDNPin, LOW); // Disable frontLaser
  digitalWrite(leftLaserSHDNPin, LOW); // Disable leftLaser
  digitalWrite(rightLaserSHDNPin, LOW); // Disable rightLaser
  
  pinMode(leftIPin1, OUTPUT);
  pinMode(leftIPin2, OUTPUT);
  pinMode(leftSpeedPin, OUTPUT);
  pinMode(rightIPin1, OUTPUT);
  pinMode(rightIPin2, OUTPUT);
  pinMode(rightSpeedPin, OUTPUT);
  
  forward(20,20);
  Wire.begin();
  
  readSensors(); // Called so that first readSensors() in loop() returns valid prev_front/left/right
}

///////////////////////////////////////////////

void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(leftIPin1,  !leftInvert);
  digitalWrite(leftIPin2,  leftInvert);
  digitalWrite(rightIPin1, rightInvert);
  digitalWrite(rightIPin2, !rightInvert);
  analogWrite(leftSpeedPin,  leftSpeed);
  analogWrite(rightSpeedPin, rightSpeed);
}
void backward(int leftSpeed, int rightSpeed) {
  digitalWrite(leftIPin1,  leftInvert);
  digitalWrite(leftIPin2,  !leftInvert);
  digitalWrite(rightIPin1, !rightInvert);
  digitalWrite(rightIPin2, rightInvert);
  analogWrite(leftSpeedPin,  leftSpeed);
  analogWrite(rightSpeedPin, rightSpeed);
}

void stop() {
  digitalWrite(leftSpeedPin,  LOW);
  digitalWrite(rightSpeedPin, LOW);
}


// Adds 3ms delay
void readSensors() {
  prev_front = temp_front;
  prev_left = temp_left;
  prev_right = temp_right;
  
  // Enable left laser
  digitalWrite(leftLaserSHDNPin, HIGH); // enabling leftLaser
  // Print left laser value
  lasers.begin();
  delayUpdateTimers(1);
  temp_left = lasers.readRange();
  printf("\tleftLaser: %3d    ", temp_left);
  digitalWrite(leftLaserSHDNPin, LOW);  // Disable leftLaser
  
  // Enable front laser
  digitalWrite(frontLaserSHDNPin, HIGH); // enabling frontLaser
  // Print front laser value
  lasers.begin(); // Notice: every time laser is enabled, have to call begin()
  delayUpdateTimers(1);
  temp_front = lasers.readRange();
  printf("frontLaser: %3d    ", temp_front);
  digitalWrite(frontLaserSHDNPin, LOW);  // Disable frontLaser
  
  // Enable right laser
  digitalWrite(rightLaserSHDNPin, HIGH); // enabling rightLaser
  // Print right laser value
  lasers.begin();
  delayUpdateTimers(1);
  temp_right = lasers.readRange();
  printf("rightLaser: %3d\n", lasers.readRange());
  digitalWrite(rightLaserSHDNPin, LOW);  // Disable rightLaser
}

///////////////////////////////////////////////

void keepInCenter();
void keepAtLeft();

void loop() {
  readSensors();
  forward(SPEED, SPEED);
  
//  if(temp_front < 100){
//    stop();
//    delayUpdateTimers(1000);
//  }
  
//  else { // If front doesn't see the wall...
    if (timeSinceIntersect >= MIN_INTERSECT_DELAY &&
        (temp_left - prev_left > INTERSECTION_CHANGE
        || temp_right - prev_right > INTERSECTION_CHANGE)) {
      stop();
      printf("At intersection\n");
      delayUpdateTimers(100);
    } else {
      keepInCenter();
    }
//  }
//  delayUpdateTimers(50);
}

///////////////////////////////////////////////////

/* Repeatedly call this function to stick to the left wall.
 */
void keepAtLeft() {
  if(temp_left > 130){ // Too far from left, turn left
    analogWrite(leftSpeedPin,SPEED - correction);
    delayUpdateTimers(100);
  }
  else if(temp_left < 70){ // Too close to left, turn right
    analogWrite(rightSpeedPin,SPEED - correction);
    delayUpdateTimers(100);
  }
}

/* Repeatedly call this function to stay at the center of the road.
 */
void keepInCenter() {
  if(temp_right < 120){ // Prev = 90
    analogWrite(leftSpeedPin,SPEED - correction);
//    delayUpdateTimers(400);
//    analogWrite(leftSpeedPin,SPEED);
//    while(temp_front < 255){
//      readSensors();      
//    }
//      printf("trightLaser: %d \n", SPEED);
    delayUpdateTimers(100);
//    analogWrite(leftSpeedPin,SPEED);
  }
  
  if(temp_left < 120){
    analogWrite(rightSpeedPin,SPEED - correction);
    delayUpdateTimers(100);
//    analogWrite(rightSpeedPin, SPEED);
    
//    while(temp_front < 255){
//      readSensors();
//    }
//    printf("tr: %d \n", SPEED);
  }
}


