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

int leftIPin1 = 12;
int leftIPin2 = 13;
int leftSpeedPin = 10; // enable motor A
boolean leftInvert = true;

int rightIPin1 = 8;
int rightIPin2 = 11;
int rightSpeedPin = 9; // Enable motor B
boolean rightInvert = false;

int SPEED = 100;
int correction = 80;
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
  
  pinMode(leftIPin1, OUTPUT);
  pinMode(leftIPin2, OUTPUT);
  pinMode(leftSpeedPin, OUTPUT);
  pinMode(rightIPin1, OUTPUT);
  pinMode(rightIPin2, OUTPUT);
  pinMode(rightSpeedPin, OUTPUT);
  
  forward(20,20);
  Wire.begin();
}

void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(leftIPin1,  !leftInvert);
  digitalWrite(leftIPin2,  leftInvert);
  digitalWrite(rightIPin1, rightInvert);
  digitalWrite(rightIPin2, !rightInvert);
  analogWrite(leftSpeedPin,  leftSpeed);
  analogWrite(rightSpeedPin, rightSpeed);
}
void stop() {
  digitalWrite(leftSpeedPin,  LOW);
  digitalWrite(rightSpeedPin, LOW);
}

/*
void wave_forward(int amplitude_right,int amplitudeleft){
   wave_offset = 0;
  if(amplitude_right < amplitude_left) ///
      wave_offset = amplitude_left - amplitude_right;
  mod_speed = SPEED - wave_offset + ( sin(wave * (PI / 180)) * amplitude_right);
  mod_speed_right = (int)mod_speed;
  
  wave_offset = 0;
  if(amplitude_right > amplitude_left) 
      wave_offset = amplitude_right - amplitude_left;
  mod_speed = SPEED - wave_offset + ( sin(wave * (PI / 180)) * amplitude_left);
  mod_speed_left = (int)mod_speed;
  
  analogWrite(leftSpeedPin,mod_speed_left);
  analogWrite(rightSpeedPin, mod_speed_right);
  printf("delay: %d %d \n", mod_speed_right, mod_speed_left);
  delay(delay_time);
  
}*/

// Adds 3ms delay
void readSensors() {
  //printf("looping\n");
  // Enable laser 1
  digitalWrite(frontLaserSHDNPin, HIGH); // enabling frontLaser
  // Print laser 1 value
  lasers.begin(); // Notice: every time laser is enabled, have to call begin()
  delay(1);
  temp_front = lasers.readRange();
  printf("\tfrontLaser: %d\n", temp_front);
  
  digitalWrite(frontLaserSHDNPin, LOW);  // Disable frontLaser
//  delay(500);

  // Enable laser 2
  digitalWrite(leftLaserSHDNPin, HIGH); // enabling leftLaser
  // Print laser 2 value
  lasers.begin();
  delay(1);
  temp_left = lasers.readRange();
  printf("\tleftLaser: %d\n", temp_left);
  digitalWrite(leftLaserSHDNPin, LOW);  // Disable leftLaser
//  delay(500);
  
  // Enable laser 3
  digitalWrite(rightLaserSHDNPin, HIGH); // enabling rightLaser
  // Print laser 3 value
  lasers.begin();
  delay(1);
  temp_right = lasers.readRange();
  printf("\trightLaser: %d\n", lasers.readRange());
  digitalWrite(rightLaserSHDNPin, LOW);  // Disable rightLaser
  //delay(500);
}

void loop() {
  readSensors();
  analogWrite(leftSpeedPin,SPEED);
  analogWrite(rightSpeedPin, SPEED);
  
  if(temp_right < 60){
    analogWrite(leftSpeedPin,SPEED - correction);
    delay(400);
    analogWrite(leftSpeedPin,SPEED);
   // while(temp_front < 255){
    //  readSensors();      
   // }
    //printf("trightLaser: %d \n", SPEED);
    //delay(400);
    //analogWrite(leftSpeedPin,SPEED);
  }
  
  if(temp_left < 70){
    analogWrite(rightSpeedPin,SPEED - correction);
    delay(400);
    analogWrite(rightSpeedPin, SPEED);
  
    //while(temp_front < 255){
    //  readSensors();
    //}
    //printf("tr: %d \n", SPEED);
  }
  
  if(temp_front < 100){
    stop();
  }
  delay(50);
  /*
  if(wave >= 270){
     wave = -90;
     //delay_time = 24;
     //delay(1000);
     amplitude_left = 20;
     amplitude_right = 20;
     SPEED = 35;
     
     digitalWrite(rightLaserSHDNPin, HIGH); // enabling rightLaser
     lasers.begin();
     delay(1);
     temp_range = lasers.readRange();
     printf("\trightLaser: %d \n", temp_range);
       if(temp_range < 70)
         amplitude_left = 10;
       if(temp_range > 100 && temp_range < 250)
         amplitude_right = 10;  
     digitalWrite(rightLaserSHDNPin, LOW);  // Disable rightLaser
     //wave_forward(amplitude_right, amplitude_left);
     
    digitalWrite(frontLaserSHDNPin, HIGH); // enabling frontLaser
    lasers.begin(); 
    delay(1);
    temp_range = lasers.readRange();
    printf("\tfrontLaser: %d\n", temp_range);
    digitalWrite(frontLaserSHDNPin, LOW);  // Disable frontLaser
     
       if(temp_range < 150){
         SPEED = 10;
         amplitude_right = 0;
         amplitude_left =  0;
       }
  }
  wave += 4; */
}
