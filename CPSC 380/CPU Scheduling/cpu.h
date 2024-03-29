
#include "task.h"

#ifndef CPU_H
#define CPU_H

// length of a time quantum
#define QUANTUM 10

// run the specified task for the following time slice
void run(Task *task, int slice);

#endif