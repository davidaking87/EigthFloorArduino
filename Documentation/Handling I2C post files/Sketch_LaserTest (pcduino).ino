// Looking at the Wire library, it uses this stuff to printf()
extern "C" {
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <inttypes.h>
}
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X sensor;

void setup() {
  printf("init start\n");
  pinMode(13, OUTPUT);
  sensor.begin();
  printf("init done\n");
}

char str[] = {"255"}; // Holds max value laser can output
void loop() {
  digitalWrite(13, HIGH);
  printf("range: %d\n", sensor.readRange());
  delay(500);
digitalWrite(13, LOW);
}
