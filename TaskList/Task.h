enum Task {
  STOP,
  START
};
#define NUM_TASK_TYPES 2

bool hasTask(Task t);
Task getCurrTask();
void addTask(Task t);

