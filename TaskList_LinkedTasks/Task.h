#ifndef TASK_H
#define TASK_H

class Task {
private:
  Task* nextTask;
  bool (*taskFunc)(); // Should return true when done
  bool done;

  Task(bool (*taskFunc)());
  
public:
  static Task* createTask(bool (*taskFunc)());
  static void deleteOneTask(Task* startTask); // Delete startTask, but do NOT touch the later tasks
  static void deleteAllTasks(Task* startTask); // Delete startTask, and all tasks after it

  void setNextTask(Task* nextTask);
  void runTask();
  bool isDone();

  void unsetDone(); // Warning: May cause infinite loops
};

#endif

