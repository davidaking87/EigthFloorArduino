int pinI1 = 8;
int pinI2 = 11;
int speedPinA = 9; // enable motor A
int pinI3 = 12;
int pinI4 = 13;

int speedPinB = 10; // Enable motor B
int speed = 127; // define speed of motor

void setup() {
  pinMode(pinI1, OUTPUT);
  pinMode(pinI2, OUTPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(pinI3, OUTPUT);
  pinMode(pinI4, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  
  stop();
}

////////////////////////////

void left() {
  digitalWrite(pinI4, HIGH); // Motor B clockwise
  digitalWrite(pinI3, LOW);
  digitalWrite(pinI2, HIGH); // Motor A clockwise
  digitalWrite(pinI1, LOW);
  analogWrite(speedPinA, speed);
  analogWrite(speedPinB, speed);
}

void stop() {
  digitalWrite(speedPinA, LOW);
  digitalWrite(speedPinB, LOW);
  digitalWrite(pinI4, LOW);
}

//////////////////////////////

void loop() {
  left();
  delay(2000);
  stop();
  delay(2000);
}
