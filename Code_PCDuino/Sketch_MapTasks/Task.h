/*
 * A linked list of tasks.
 */

#ifndef TASK_H
#define TASK_H

extern "C" {
  #include <stddef.h> // For NULL
}

class Task {
private:
  Task* nextTask;
  bool (*taskFunc)();
  bool done;
  
  Task(bool (*taskFunc)());
  
public:
  static Task* createTask(bool (*taskFunc)(), Task* prevTask);
  static void deleteOneTask(Task* task); // Only deletes task
  static void deleteAllTasks(Task* startTask); // Deletes task, and all Tasks that follow
  
  void setNextTask(Task* nextTask);
  void runTask();
  bool isDone();
  
  void unsetDone();
};

#endif


