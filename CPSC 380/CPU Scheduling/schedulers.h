#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include "task.h"
#include "cpu.h"

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#define TASK_SIZE 100

//keeps track of the amount of items in the myTasks array
static int currentSize = 0;

//used for storing tasks from the input file
static Task *myTasks[TASK_SIZE];

// add a task to the list 
void add(char *name, int priority, int burst);

// invoke the first come first serve scheduler
void schedule_fcfs();

// invoke the priority scheduler (lower numbers = higher priority)
void schedule_priority();

// invoke the round robin scheduler (10 second bursts)
void schedule_rr();

// invoke the shortest job first scheduler (smaller bursts go first)
void schedule_sjf();

//function to free the memory of the myTasks array
void freeMem();

#endif