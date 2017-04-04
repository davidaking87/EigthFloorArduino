/* Roll Chassis
 * 
 * Make the chassis roll forward, backward, left, and then right.
 */

// Motor A
byte pinI1 = 8;
byte pinI2 = 11;
byte speedPinA = 9; // Starts & stops Motor A
  bool invertA = false;
// Motor B
byte pinI3 = 12;
byte pinI4 = 13;
byte speedPinB = 10; // Starts & stops MotorB
  bool invertB = true;

///////////////////////
// Needs to be at least 74?
int SPEED = 80;
///////////////////////

void setup() {
  pinMode(pinI1, OUTPUT);
  pinMode(pinI2, OUTPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(pinI3, OUTPUT);
  pinMode(pinI4, OUTPUT);
  pinMode(speedPinB, OUTPUT);

  Serial.begin(9600);
  Serial.println(invertA);
  Serial.println(invertB);
  
  // put your setup code here, to run once:
  stopMotors();
//  digitalWrite(pinI4, LOW); // Make LED pin blink...
}

void loop() {
  // put your main code here, to run repeatedly:
  forwardMotors(SPEED, SPEED);
  delay(1000);
  stopMotors();
  delay(1000);
  
  backwardMotors(SPEED, SPEED);
  delay(1000);
  stopMotors();
  delay(1000);
  
  leftMotors(SPEED, SPEED);
  delay(1000);
  stopMotors();
  delay(1000);
  
  rightMotors(SPEED, SPEED);
  delay(1000);
  stopMotors();
  delay(5000);
}

/////////////////////////////

void forwardMotors(int speedA, int speedB) {
  digitalWrite(pinI1, !invertA);
  digitalWrite(pinI2, invertA);
  digitalWrite(pinI3, invertB);
  digitalWrite(pinI4, !invertB);
  analogWrite(speedPinA, speedA);
  analogWrite(speedPinB, speedB);
}
void backwardMotors(int speedA, int speedB) {
  digitalWrite(pinI1, invertA);
  digitalWrite(pinI2, !invertA);
  digitalWrite(pinI3, !invertB);
  digitalWrite(pinI4, invertB);
  analogWrite(speedPinA, speedA);
  analogWrite(speedPinB, speedB);
}

void leftMotors(int speedA, int speedB) {
  digitalWrite(pinI1, invertA);
  digitalWrite(pinI2, !invertA);
  digitalWrite(pinI3, invertB);
  digitalWrite(pinI4, !invertB);
  analogWrite(speedPinA, speedA);
  analogWrite(speedPinB, speedB);
}
void rightMotors(int speedA, int speedB) {
  digitalWrite(pinI1, !invertA);
  digitalWrite(pinI2, invertA);
  digitalWrite(pinI3, !invertB);
  digitalWrite(pinI4, invertB);
  analogWrite(speedPinA, speedA);
  analogWrite(speedPinB, speedB);
}

void stopMotors() {
  digitalWrite(speedPinA, LOW);
  digitalWrite(speedPinB, LOW);
}