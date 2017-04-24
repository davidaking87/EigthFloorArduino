enum Task {
  STOP,
  START
};
#define NUM_TASK_TYPES 2

void addTask(Task t);
void removeTask(Task t);

bool hasTask(Task t);
Task getCurrTask();

void runCurrTask();

