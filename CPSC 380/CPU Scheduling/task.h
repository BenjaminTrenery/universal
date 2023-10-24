/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

//keeps track of what unique ids
static int tidIDNum = 0; 

// representation of a task
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
} Task;

#endif
