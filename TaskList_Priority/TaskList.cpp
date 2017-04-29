// function pointers:
//    http://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers#252750
//    http://forum.arduino.cc/index.php?topic=204418.0

#include "TaskList.h"

TaskList::TaskList(void (**taskFuncs)(), int numTasks) {
  this->numTasks = numTasks;
  this->taskFuncs = taskFuncs;
  for (int i=0; i < MAX_NUM_TASK_TYPES; ++i)
    runningTasks[i] = false;
}

//////////////////////

void TaskList::addTask(int taskIndex) {
  if (taskIndex >= 0 && taskIndex < numTasks)
    runningTasks[taskIndex] = true;
}
void TaskList::removeTask(int taskIndex) {
  if (taskIndex >= 0 && taskIndex < numTasks)
    runningTasks[taskIndex] = false;
}

//////////////////////

bool TaskList::hasTask(int taskIndex) {
  if (taskIndex >= 0 && taskIndex < numTasks)
    return runningTasks[taskIndex];
  return false; // If not an actual task
}
// Get the current task.
// If no tasks running, returns -1.
int TaskList::getCurrTask() {
  for (int index = numTasks-1; index >= 0; --index) {
    if (runningTasks[index])
      return index;
  }
  return -1;
}

//////////////////////

void TaskList::runCurrTask() {
  int taskIndex = getCurrTask();
  if (taskIndex > -1) // If task exists, run the current task
    taskFuncs[taskIndex]();
}

