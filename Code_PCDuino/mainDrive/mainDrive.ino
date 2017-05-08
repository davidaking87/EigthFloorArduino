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

byte leftLEDPin = 5;
byte rightLEDPin = 6;

int leftIPin1 = 12;
int leftIPin2 = 13;
int leftSpeedPin = 10; // enable motor A
boolean leftInvert = false;

int rightIPin1 = 8;
int rightIPin2 = 11;
int rightSpeedPin = 9; // Enable motor B
boolean rightInvert = true;

/* Before battery change (~6.8/7V), 50 speed worked
   After battery change: (9.8V down to 8.9V), 20-50 too high, 15 too low
     at 8.8V, 17 speed, 10 correction works.
         turnCorrection =???
     at 8.6V, 17 speed too low!
*/
int SPEED = 20;
int correction = 10;
int turnCorrection = 11; // Might not be done in the demo
int temp_front;
  int prev_front;
int temp_right;
  int prev_right;
int temp_left;
  int prev_left;

int INTERSECTION_CHANGE = 70; // If distance changes 100mm b/w readings, at intersection
// Going really slowly, so delay = 5 seconds
int MIN_INTERSECT_DELAY = 0; // Min time to wait between intersections
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
  
  pinMode(leftLEDPin, OUTPUT);
  pinMode(rightLEDPin, OUTPUT);
  
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

void left(int leftSpeed, int rightSpeed) {
  digitalWrite(leftIPin1,  leftInvert);
  digitalWrite(leftIPin2,  !leftInvert);
  digitalWrite(rightIPin1, rightInvert);
  digitalWrite(rightIPin2, !rightInvert);
  analogWrite(leftSpeedPin,  leftSpeed);
  analogWrite(rightSpeedPin, rightSpeed);
}

void stop() {
  // At high voltages, robot doesn't stop. So, tell it to go as slow as possible
  analogWrite(leftSpeedPin,  0);
  analogWrite(rightSpeedPin, 0);
  digitalWrite(leftSpeedPin,  LOW);
  digitalWrite(rightSpeedPin, LOW);
}

///////////////////////////////////////////////

Task* turnTask = NULL;

void loop() {
  readSensors();
  // Reset drive & LEDs to a known state:
  //    going forward & LEDs off
  forward(SPEED, SPEED);
  digitalWrite(leftLEDPin, LOW);
  digitalWrite(rightLEDPin, LOW);
  
  if(temp_front < 100){ // Stop when it hits a wall
    // Turn on both LEDs when bot detects a wall in front
    digitalWrite(leftLEDPin, HIGH);
    digitalWrite(rightLEDPin, HIGH);
    stop();
    delay(1000);
  }
  
  else { // If front doesn't see the wall...
    // When you see an intersection...
    if (millis() -prevIntersectTime >= MIN_INTERSECT_DELAY &&
        (temp_left - prev_left > INTERSECTION_CHANGE
        || temp_right - prev_right > INTERSECTION_CHANGE)) {
      // Make a task to turn left
//      if (turnTask == NULL) {
//        turnTask = Task::createTask(turnLeft, NULL);
//        turnTask->setNextTask(currTask);
//        currTask = turnTask; // & run turnTask
//      }
      
      stop();
      printf("At intersection\n");
      // Blink both lights at an intersection
      prevIntersectTime = millis();
      while (millis() - prevIntersectTime < 1000) {
        blinkRightLED(100);
        blinkLeftLED(100);
        delay(1);
      }
      
      prevIntersectTime = millis();
      forward(SPEED, SPEED);
    }
    
    // When done turning, delete the task
    if (turnTask != NULL && turnTask->isDone()) {
      currTask = turnTask->getNextTask(); // Make sure to keep pointer to the next task...
      Task::deleteOneTask(turnTask);
      turnTask = NULL;
    }
    currTask->runTask();
  }
//  delay(50);
}

///////////////////////////////////////////////////

// Adds 3+ ms delay
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

/* Repeatedly call this function to stick to the left wall.
   If we knew where we were on the map, this would be very
       effective @ the curved turn.
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

/* This code makes the robot turn to the left path.
      This hasn't been perfected, so it isn't called.
*/
bool turnLeft(bool hasInit){
  // Turn on LEDs to show: "still at intersection"
//  digitalWrite(leftLEDPin, HIGH);
//  digitalWrite(rightLEDPin, HIGH);
  
  while(temp_front > 250){
    analogWrite(leftSpeedPin,SPEED - turnCorrection);
    readSensors();
  }
   while(temp_front < 252){
    analogWrite(leftSpeedPin,SPEED - turnCorrection);
     readSensors();
   }
  return true;
}
/* Repeatedly call this function to stay at the center of the road.
 */
bool keepInCenter(bool hasInit) {
  // Used to have distance check of 90
  if(temp_right < 80){
    analogWrite(leftSpeedPin,SPEED - correction);
    digitalWrite(leftLEDPin, HIGH);
     printf("LeftMotor: %d \n", SPEED - correction);
    delay(100);
//    analogWrite(leftSpeedPin,SPEED);
  }
  else if(temp_right < 120){
    analogWrite(leftSpeedPin,SPEED - correction/2);
    blinkLeftLED(0);
    printf("LeftMotor: %d \n", SPEED - correction/2);
    delay(100);
//    analogWrite(leftSpeedPin,SPEED);
  }
  
  else if(temp_left < 80){
    analogWrite(rightSpeedPin,SPEED - correction);
    digitalWrite(rightLEDPin, HIGH);
    printf("rightMotor: %d \n", SPEED - correction);
    delay(100);
  }
  else if(temp_left < 120){ 
    analogWrite(rightSpeedPin,SPEED - correction/2);
    blinkRightLED(0);
    printf("rightMotor: %d \n", SPEED - correction/2);    
    delay(100);
  }
  
  // Code below tried to keep robot in center @ intersection...
  // Turns out, the sensors can't detect far enough for the code to work
  /*
  if (temp_left < 10) {
    backward(SPEED, SPEED);
    delay()
  } else if (temp_right < 10) {
    backward(SPEED, SPEED);
  }
  
  else if(temp_left - temp_right > 40){ // Too far from the left
    analogWrite(leftSpeedPin,SPEED - correction);
    digitalWrite(leftLEDPin, HIGH);
    printf("LEFT MOTOR: %d \n", SPEED - correction);
//    delay(100);
  }
  else if(temp_right - temp_left > 40){  // Too far from right
    analogWrite(rightSpeedPin,SPEED - correction);
    digitalWrite(rightLEDPin, HIGH);
    printf("RIGHT MOTOR: %d \n", SPEED - correction);    
//    delay(100);
  }
  
  else if(temp_left - temp_right > 20){ // Too far from the left
    analogWrite(leftSpeedPin,SPEED - correction/2);
    blinkLeftLED(50);
    printf("leftMotor: %d \n", SPEED - correction/2);
//    delay(100);
  }
  else if(temp_right - temp_left > 20){  // Too far from right
    analogWrite(rightSpeedPin,SPEED - correction/2);
    blinkRightLED(50);
    printf("rightMotor: %d \n", SPEED - correction/2);    
//    delay(100);
  }
  
  else if (temp_right >= 255 -20 && temp_left >= 255 -20) {
    blinkRightLED(0);
    blinkLeftLED(0);
  }
  */
  
  return false; // For now, never end task
}

// Blink the leftLED
void blinkLeftLED(int blinkTimeMs) {
  static bool wasLEDOn = false;
  static unsigned long lastTime = blinkTimeMs -millis();
  static int prevBlinkTimeMs = blinkTimeMs;
  
  if (blinkTimeMs != prevBlinkTimeMs) { // If started new blink, reset variables & turn LED on
    wasLEDOn = false;
    lastTime = blinkTimeMs -millis();
  }
  
  if (!wasLEDOn) { // Light wasn't on, wait a bit & turn it on
    if (millis() - lastTime >= blinkTimeMs) { // If it's been long enough...
      digitalWrite(leftLEDPin, HIGH);
      wasLEDOn = true;
      lastTime = millis();
    } else { // If it's not been long enough, just keep the LED off
      digitalWrite(leftLEDPin, LOW);
    }
    
  } else { // Light was on, wait a bit & turn it off
    if (millis() - lastTime >= blinkTimeMs) { // If it's been long enough...
      digitalWrite(leftLEDPin, LOW);
      wasLEDOn = false;
      lastTime = millis();
    } else { // If it's not been long enough, just keep the LED on
      digitalWrite(leftLEDPin, HIGH);
    }
  }
  
  prevBlinkTimeMs = blinkTimeMs;
}
// Blink the rightLED
void blinkRightLED(int blinkTimeMs) {
  static bool wasLEDOn = false;
  static unsigned long lastTime = blinkTimeMs -millis();
  static int prevBlinkTimeMs = blinkTimeMs;
  
  if (blinkTimeMs != prevBlinkTimeMs) { // If started new blink, reset variables & turn LED on
    wasLEDOn = false;
    lastTime = blinkTimeMs -millis();
  }
  
  if (!wasLEDOn) { // Light wasn't on, wait a bit & turn it on
    if (millis() - lastTime >= blinkTimeMs) { // If it's been long enough...
      digitalWrite(rightLEDPin, HIGH);
      wasLEDOn = true;
      lastTime = millis();
    } else { // If it's not been long enough, just keep the LED off
      digitalWrite(rightLEDPin, LOW);
    }
    
  } else { // Light was on, wait a bit & turn it off
    if (millis() - lastTime >= blinkTimeMs) { // If it's been long enough...
      digitalWrite(rightLEDPin, LOW);
      wasLEDOn = false;
      lastTime = millis();
    } else { // If it's not been long enough, just keep the LED on
      digitalWrite(rightLEDPin, HIGH);
    }
  }
  
  prevBlinkTimeMs = blinkTimeMs;
}
