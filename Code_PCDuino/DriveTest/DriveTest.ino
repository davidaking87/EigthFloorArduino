// Motor1 = right, Motor2 = left

int leftIPin1 = 12;
int leftIPin2 = 13;
int leftSpeedPin = 10; // enable motor A
boolean leftInvert = true;

int rightIPin1 = 8;
int rightIPin2 = 11;
int rightSpeedPin = 9; // Enable motor B
boolean rightInvert = false;

void setup() {
  pinMode(leftIPin1, OUTPUT);
  pinMode(leftIPin2, OUTPUT);
  pinMode(leftSpeedPin, OUTPUT);
  pinMode(rightIPin1, OUTPUT);
  pinMode(rightIPin2, OUTPUT);
  pinMode(rightSpeedPin, OUTPUT);
  stop();
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

int SPEED = 100;

void loop() {
  forward(SPEED, SPEED);
  delay(10000);
  stop();
  delay(1000);
  
  backward(SPEED, SPEED);
  delay(10000);
  stop();
  delay(1000);
  
  left(SPEED, SPEED);
  delay(1000);
  stop();
  delay(1000);
  
  right(SPEED, SPEED);
  delay(1000);
  stop();
  delay(3000);
}

