#include "Task.h"
extern "C" {
  #include <stdio.h>
}

Task::Task(bool (*taskFunc)(bool hasInit)) {
  this->taskFunc = taskFunc;
  this->nextTask = NULL;
  this->done = false;
  this->hasInit = false;
}

/////////////////////

Task* Task::createTask(bool (*taskFunc)(bool hasInit), Task* prevTask) {
  Task* newTask = new Task(taskFunc);
  if (prevTask != NULL)
    prevTask->setNextTask(newTask);
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
//    printf("tasked\n");
    if (this->nextTask != NULL)
      this->nextTask->runTask();
  } else { // Not done... run this task
//    printf("tasker\n");
    if (this->taskFunc != NULL)
      this->done = this->taskFunc(this->hasInit);
  }
  this->hasInit = true;
}

bool Task::isDone() {
  return this->done;
}
void Task::unsetDone() {
  this->done = false;
  this->hasInit = false; // Re-initialize task
}

