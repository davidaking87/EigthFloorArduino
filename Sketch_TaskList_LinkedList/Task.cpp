#include "Task.h"
#include <stddef.h>
extern "C" {
  #include <stdio.h>
}

Task::Task(bool (*taskFunc)()) {
  this->taskFunc = taskFunc;
  this->nextTask = NULL;
  this->done = false;
}

/////////////////////

Task* Task::createTask(bool (*taskFunc)()) {
  Task* newTask = new Task(taskFunc);
  return newTask;
}
void Task::deleteOneTask(Task* task) {
  if (task != NULL) {
    task->taskFunc = NULL;
    task->nextTask = NULL;
  }
}
void Task::deleteAllTasks(Task* startTask) {
  if (startTask != NULL) {
    if (startTask->nextTask != NULL)
      Task:deleteAllTasks(startTask->nextTask);
    // Finally, delete this task
    Task::deleteOneTask(startTask);
  }
}

void Task::setNextTask(Task* nextTask) {
  this->nextTask = nextTask;
}

void Task::runTask() {
  if (this->done) { // If this is done, run the next task
    if (this->nextTask != NULL)
      this->nextTask->runTask();
  } else { // Not done... run this task
    if (this->taskFunc != NULL)
    this->done = this->taskFunc();
  }
}

bool Task::isDone() {
  return this->done;
}
void Task::unsetDone() {
  this->done = false;
}
