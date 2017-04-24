#include "Task.h"

void setup() {
  addTask(Task.STOP);
}

void loop() {
  runCurrTask();
  delay(100); // Wait a bit before printing more
}

