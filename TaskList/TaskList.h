/*
 * A TaskList will have a list of tasks the user can run.
 *    If multiple tasks are set at the same time,
 *    the highest priority one will be ran.
 */
#ifndef TASKLIST_H
#define TASKLIST_H

#define MAX_NUM_TASK_TYPES 2

class TaskList {
private:
  int numTasks;
  void (**taskFuncs)(); // Pointers to functions
  bool runningTasks[MAX_NUM_TASK_TYPES];
  
public:
  TaskList(void (**taskFuncs)(), int numTasks);
  void addTask(int taskIndex);
  void removeTask(int taskIndex);
  
  bool hasTask(int taskIndex);
  int getCurrTask();
  
  void runCurrTask();
};

#endif

