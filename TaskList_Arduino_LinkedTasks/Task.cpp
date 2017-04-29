#include "Task.h"
#include <stddef.h>

Task::Task(bool (*taskFunc)()) {
    this->taskFunc = taskFunc;
    this->nextTask = NULL;
    this->done = false;
}

//***********************************/

Task* Task::createTask(bool (*taskFunc)()) {
    Task* newTask = new Task(taskFunc); // Dynamically create the new task
    return newTask;
}
void Task::deleteOneTask(Task* startTask) {
  if (startTask != NULL) {
    startTask->taskFunc = NULL;
    startTask->nextTask = NULL;
  }
  delete startTask; // Clear dynamic memory
}
void Task::deleteAllTasks(Task* startTask) {
  if (startTask != NULL) {
    // Delete later tasks first
    if (startTask->nextTask != NULL)
      Task::deleteAllTasks(startTask->nextTask);
    // Finally, delete this task
    Task::deleteOneTask(startTask);
  }
}

//***********************************/

void Task::setNextTask(Task* nextTask) {
  this->nextTask = nextTask;
}

void Task::runTask() {
  if (this->done) { // Done... Run next task
    if (this->nextTask != NULL) {
      this->nextTask->runTask();
    }
    
  } else { // Not done... Run this task
    if (this->taskFunc != NULL)
      this->done = taskFunc();
  }
}
bool Task::isDone() {
  return this->done;
}

void Task::unsetDone() {
  this->done = false;
}

