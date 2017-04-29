/* TaskList
 * 
 * A TaskList will have a list of tasks the user can run.
 *    If multiple tasks are set at the same time,
 *    the highest priority one will be ran.
 *****************
 * In this demo, there are two tasks: Stop, Start
 * Stop = first index = lowest priority
 *    This function prints stuff 3 times, then sets Start task.
 *    (This task is always set)
 * Start = second index = higher priority
 *    This function prints stuff 3 times, then unsets Start task.
 * 
 * Notice: static variables were used to retain state b/w function calls.
 */

//extern "C" { // To use printf on pcduino
//  #include <stdio.h>
//}
#include "TaskList.h"

//////////////////////
#define TASK_STOP 0
#define TASK_START 1

void taskStop();
void taskStart();

void (*taskFuncs[2])() = {
  taskStop,
  taskStart
};
//////////////////////

TaskList taskList(taskFuncs, 2);
void setup() {
  Serial.begin(9600);
  taskList.addTask(TASK_STOP);
}

void loop() {
  taskList.runCurrTask();
  delay(100); // Wait a bit before printing more
}

//////////////////////

void taskStop() {
  static int counter = 0;
//  printf("stop %d\n", counter);
  Serial.print("stop");
  Serial.println(counter);
  if ((++counter)%3 == 0) { // Run task 3 times, then reset
//    counter = 0;
    taskList.addTask(TASK_START);
  } else if (counter%3 == 1) // Wait a bit before printing more
    delay(1000);
}
void taskStart() {
  static int counter = 0;
//  printf("start %d\n", counter);
  Serial.print("\tstart");
  Serial.println(counter);
  if (++counter >= 3) { // Run task 3 times, then reset
    counter = 0;
    taskList.removeTask(TASK_START);
  }
}
