/* "Glue code" between pathfinding & drive code.
 * This translates MOVES into Task function calls.
 */

#include "Task.h"
#include "Map.h"

extern "C" {
  #include <stdio.h> // For printf (PCDuino)
}

Task* generateTasks(MOVE* moveArr, int arrSize, Task*& lastMoveTask);

Task* firstMoveTask = NULL; // Is set by generateTasks
Task* lastMoveTask = NULL; // Is set by generateTasks
MOVE moveList[] = {
  FORWARD, FORWARD, LEFT, FORWARD, RIGHT,
  RIGHT, REVERSE
};
#define MOVE_LIST_LEN 7

void setup() {
  firstMoveTask = generateTasks(moveList, MOVE_LIST_LEN, lastMoveTask);
}

void loop() {
  if (!lastMoveTask->isDone()) { // When not done...
//    printf("\ttasking\n");
    firstMoveTask->runTask();
    
  } else { // All tasks done...
    printf("-- Tasks done --\n");
    delay(5000);
  }
  delay(50);
}

/////////////////////////////////
bool forwardToIntersection();
bool turnToLeftPath();
bool turnToRightPath();
bool turnToBackPath();

bool blankFunc() {  printf("blank\n"); return true;  } // First task in all lists...
bool lastMoveFunc() { // Last task in all lists...
  printf("MOVE DONE\n");
  return true;
}

/*
 * Convert the array of MOVES to a linked list of Tasks.
 *    Returns startTask.
 * Afterward, you can execute all the Tasks
 *    with startTask->runTask()
 */
Task* generateTasks(MOVE* moveArr, int arrSize, Task*& lastMoveTask) {
  Task* firstTask = Task::createTask(blankFunc, NULL);
  Task* prevTask = firstTask;
  int i;
  for (i=0; i < arrSize; ++i) {
    switch(moveArr[i]) {
    case FORWARD: // Don't do anything extra...
      break;
    case LEFT: // Before going straight, turn left
      prevTask = Task::createTask(turnToLeftPath, prevTask);
      break;
    case RIGHT: // Before going straight, turn right
      prevTask = Task::createTask(turnToRightPath, prevTask);
      break;
    case REVERSE: // Before going straight, turn around
      prevTask = Task::createTask(turnToBackPath, prevTask);
      break;
    }
    // After above, go straight
    prevTask = Task::createTask(forwardToIntersection, prevTask);
  }
  // Make last task
  if (lastMoveTask != NULL) {
    printf("WARNING: lastMoveTask already existed!\n");
    Task::deleteAllTasks(lastMoveTask);
  }
  lastMoveTask = Task::createTask(lastMoveFunc, prevTask);
  return firstTask;
}

/////////////////////////////////

// Should return true when at next intersection.
bool forwardToIntersection() {
  printf("drive forward\n");
  return true;
}
// Should return true when turned to the path at the left
bool turnToLeftPath() {
  printf("\tturn LEFT\n");
  return true;
}
// Should return true when turned to the path at right
bool turnToRightPath() {
  printf("\tturn RIGHT\n");
  return true;
}
// Should return true when robot turned around.
bool turnToBackPath() {
  // Print "turn around" 2x, then finish
  static int counter = 0;
  printf("\tturn AROUND...\n");
  if (++counter >= 2) {
    counter = 0;
    return true;
  }
  return false;
}
