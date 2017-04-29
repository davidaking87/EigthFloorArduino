/*
 * This is an alternate method of Task Lists.
 * Instead of having prioritized tasks,
 *    you can set one Task to run...
 *    then another... and another...
 *    until all Tasks are complete.
 */

#include "Task.h"

//* Define Tasks *************************/

Task* straightTask;
Task* leftTask;

// Straight task prints straight three times.
bool printStraight() {
  static int counter = 0;
  Serial.print("straight ");
  Serial.println(counter);
  
  if (++counter >= 3) {
    counter = 0;
//    leftTask->unsetDone(); // Infinitely loop tasks
    return true;
  }
  return false;
}

// Left task prints left twice, then stops
bool printLeft() {
  static int counter = 0;
  Serial.print("\tleft ");
  Serial.println(counter);
  
  if (++counter >= 2) {
    counter = 0;
//    straightTask->unsetDone(); // Infinitely loop tasks
//    delay(1000);
    return true;
  }
  return false;
}

//****************************/

void setup() {
  straightTask = Task::createTask(printStraight);
  leftTask = Task::createTask(printLeft);
  
  straightTask->setNextTask(leftTask);
  leftTask->setNextTask(straightTask);
  Serial.begin(9600);
}

void loop() {
  if (!straightTask->isDone() || !leftTask->isDone()) {
    straightTask->runTask();
  } else {
    Serial.println("tasks done!");
    delay(5000);
  }
    delay(100);
}
