// function pointers: http://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers#252750

extern "C" { // To use printf on pcduino
  #include <stdio.h>
}

taskStop() {
  static int counter = 0;
  printf("stop %d\n", counter);
//  Serial.print("stop");
//  Serial.println(counter);
  if (++counter >= 3) { // Run task 3 times, then reset
    counter = 0;
    addTask(Task.START);
  }
}
taskStart() {
  static int counter = 0;
  printf("start %d\n", counter);
//  Serial.print("start");
//  Serial.println(counter);
  if (++counter >= 3) { // Run task 3 times, then reset
    counter = 0;
    removeTask(Task.START);
  }
}

//////////////////////

void (*taskFuncs[NUM_TASK_TYPES]) = {
  taskStop,
  taskStart
};
bool tasks[NUM_TASK_TYPES];
// Init
int index;
for (index=0; index < NUM_TASK_TYPES; ++index)
  tasks[index] = false;

//////////////////////

void addTask(Task t) {
  tasks[t] = true;
}
void removeTask(Task t) {
  tasks[t] = false;
}

//////////////////////

bool hasTask(Task t) {
  return tasks[t];
}
// Get the current task.
// If no tasks running, returns -1.
Task getCurrTask() {
  int index;
  for (index = NUM_TASK_TYPES-1; index >= 0; --index) {
    if (tasks[index])
      return index;
  }
  return -1;
}

//////////////////////

void runCurrTask() {
  Task t = getCurrTask();
  if (t > -1) // If task exists, run the current task
    (*taskFuncs[t])();
  else
    printf("no task to run\n");
//    Serial.println("no task to run");
}

