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
#include "Task.h"

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

int INTERSECTION_CHANGE = 100; // If distance changes 100mm b/w readings, at intersection
// Going really slowly, so delay = 5 seconds
int MIN_INTERSECT_DELAY = 5100; // Min time to wait between intersections
unsigned long prevIntersectTime = millis() -MIN_INTERSECT_DELAY; // Can detect intersection immediately

///////////////////////////////////////////////

void readSensors();
bool keepAtLeft(bool hasInit);
bool keepInCenter(bool hasInit);

///////////////////////////////////////////////

Task* currTask = Task::createTask(keepInCenter, NULL);

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
  
  stop();
  Wire.begin(); // For lasers
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

///////////////////////////////////////////////

void loop() {
  readSensors();
  forward(SPEED, SPEED);
  
//  if(temp_front < 100){
//    stop();
//    delay(1000);
//  }
  
//  else { // If front doesn't see the wall...
    if (millis() -prevIntersectTime >= MIN_INTERSECT_DELAY &&
        (temp_left - prev_left > INTERSECTION_CHANGE
        || temp_right - prev_right > INTERSECTION_CHANGE)) {
      stop();
      prevIntersectTime = millis();
      printf("At intersection\n");
      delay(100);
    } else {
      currTask->runTask();
    }
//  }
//  delay(50);
}

///////////////////////////////////////////////////

// Adds 3ms delay
void readSensors() {
  prev_front = temp_front;
  prev_left = temp_left;
  prev_right = temp_right;
  
  // Enable left laser
  digitalWrite(leftLaserSHDNPin, HIGH); // ena bling leftLaser
  // Print left laser value
  lasers.begin();
  delay(1);
  temp_left = lasers.readRange();
  printf("\tleftLaser: %3d    ", temp_left);
  digitalWrite(leftLaserSHDNPin, LOW);  // Disable leftLaser
  
  // Enable front laser
  digitalWrite(frontLaserSHDNPin, HIGH); // enabling frontLaser
  // Print front laser value
  lasers.begin(); // Notice: every time laser is enabled, have to call begin()
  delay(1);
  temp_front = lasers.readRange();
  printf("frontLaser: %3d    ", temp_front);
  digitalWrite(frontLaserSHDNPin, LOW);  // Disable frontLaser
  
  // Enable right laser
  digitalWrite(rightLaserSHDNPin, HIGH); // enabling rightLaser
  // Print right laser value
  lasers.begin();
  delay(1);
  temp_right = lasers.readRange();
  printf("rightLaser: %3d\n", lasers.readRange());
  digitalWrite(rightLaserSHDNPin, LOW);  // Disable rightLaser
}

typedef enum {LASER_TOO_CLOSE, LASER_OK, LASER_TOO_FAR} laserState;

/* Repeatedly call this function to stick to the left wall.
 */
bool keepAtLeft(bool hasInit) {
  if(temp_left > 130){ // Too far from left, turn left
    analogWrite(leftSpeedPin,SPEED - correction);
    delay(100);
  }
  else if(temp_left < 70){ // Too close to left, turn right
    analogWrite(rightSpeedPin,SPEED - correction);
    delay(100);
  }
  return false; // For now, never end task
}

/* Repeatedly call this function to stay at the center of the road.
 */
bool keepInCenter(bool hasInit) {
  if(temp_right < 80){ // Prev = 90
    analogWrite(leftSpeedPin,SPEED - correction);
     printf("LeftMotor: %d \n", SPEED - correction);
    delay(100);
//    analogWrite(leftSpeedPin,SPEED);
  }
  else{
      if(temp_right < 120){ // Prev = 90
        analogWrite(leftSpeedPin,SPEED - correction/2);
        printf("LeftMotor: %d \n", SPEED - correction/2);
        delay(100);
    //    analogWrite(leftSpeedPin,SPEED);
      }
  }
  
  if(temp_left < 80){
    analogWrite(rightSpeedPin,SPEED - correction);
    printf("rightMotor: %d \n", SPEED - correction);
    delay(100);
  }
  else{
    if(temp_left < 120){ 
        analogWrite(rightSpeedPin,SPEED - correction/2);
        printf("LeftMotor: %d \n", SPEED - correction/2);    
        delay(100);
    }
  
  }
  return false; // For now, never end task
}


