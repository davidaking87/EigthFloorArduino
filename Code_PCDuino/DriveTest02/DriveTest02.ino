#include <math.h>

#define PI 3.14159264 
// Motor1 = right, Motor2 = left

int leftIPin1 = 12;
int leftIPin2 = 13;
int leftSpeedPin = 10; // enable motor A
boolean leftInvert = true;

int rightIPin1 = 8;
int rightIPin2 = 11;
int rightSpeedPin = 9; // Enable motor B
boolean rightInvert = false;

double mod_speed;
int mod_speed_left;
int mod_speed_right;
int wave = 1;

void setup() {
  pinMode(leftIPin1, OUTPUT);
  pinMode(leftIPin2, OUTPUT);
  pinMode(leftSpeedPin, OUTPUT);
  pinMode(rightIPin1, OUTPUT);
  pinMode(rightIPin2, OUTPUT);
  pinMode(rightSpeedPin, OUTPUT);
  forward(50,50);
}

////////////////////////////

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
void right(int leftSpeed, int rightSpeed) {
  digitalWrite(leftIPin1,  !leftInvert);
  digitalWrite(leftIPin2,  leftInvert);
  digitalWrite(rightIPin1, !rightInvert);
  digitalWrite(rightIPin2, rightInvert);
  analogWrite(leftSpeedPin,  leftSpeed);
  analogWrite(rightSpeedPin, rightSpeed);
}

void stop() {
  digitalWrite(leftSpeedPin,  LOW);
  digitalWrite(rightSpeedPin, LOW);
}

//////////////////////////////

int SPEED = 30;
//int dir = 10;

void loop() {
  
  mod_speed = SPEED + ( sin(wave * (PI / 180)) * 5) - 2;
  mod_speed_right = (int)mod_speed;
  mod_speed = SPEED + ( sin(wave * (PI / 180)) * 7);
  mod_speed_left = (int)mod_speed;
  analogWrite(leftSpeedPin,mod_speed_left);
  analogWrite(rightSpeedPin, mod_speed_right);
  printf("modspeed: %d \n",mod_speed_right);
  delay(6);
  
  if(wave >= 360)
     wave = 1;
  wave += 4;
}
