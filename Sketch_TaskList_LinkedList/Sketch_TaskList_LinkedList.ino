#include "Task.h"
extern "C" {
  #include <stdio.h>
}

////////////////////

Task* straightTask;
Task* leftTask;

// Straight prints straight 3 times
bool printStraight() {
  static int counter = 0;
  printf("straight %d\n", ++counter);
  if (counter >= 3) {
    counter = 0;
    return true;
  }
  return false;
}

// Left prints left twice, then stops
bool printLeft() {
  static int counter = 0;
  printf("\tleft %d\n", ++counter);
  if (counter >= 2) {
    counter = 0;
//    printf("\tleft DONE\n");
    return true;
  }
  return false;
}

///////////////////////////

void setup() {
  straightTask = Task::createTask(printStraight);
  leftTask = Task::createTask(printLeft);
  straightTask->setNextTask(leftTask);
}

void loop() {
  if (!straightTask->isDone() || !leftTask->isDone()) {
//    printf("task?");
    straightTask->runTask();
  } else {
    printf("tasks done\n");
    delay(5000);
  }
  delay(100);
}
