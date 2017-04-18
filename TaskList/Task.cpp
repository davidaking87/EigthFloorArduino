bool tasks[NUM_TASK_TYPES];
int index;
for (index=0; index < NUM_TASK_TYPES; ++index)
  tasks[index] = false;

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

void addTask(Task t) {
  tasks[t] = true;
}

