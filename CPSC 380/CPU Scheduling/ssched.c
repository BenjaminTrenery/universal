/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedulers.h"
#include "schedulers.c"
#include "cpu.c"
#include "task.h"
#include "cpu.h"


#define SIZE    100

int main(int argc, char *argv[])
{
    
    FILE *in;
    char *temp;
    char task[SIZE];

    char  *name;
    int   priority;
    int   burst;

    if (argc != 3) {
        printf("Usage: %s <sched> <taskList> \n", argv[0]);
        return -1;
    }

    in = fopen(argv[2],"r");
    
    while (fgets(task,SIZE,in) != NULL) {

        temp = strdup(task);
        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));

        // create the task add the task to the scheduler's list of tasks
        add(name,priority,burst);

        free(temp);
    }

    fclose(in);

    // invoke the scheduler
    if (strncmp(argv[1], "fcfs", 4) == 0) {
       schedule_fcfs();
    } else if (strncmp(argv[1], "priority", 8) == 0) {
       schedule_priority();
    } else if (strncmp(argv[1], "rr", 2) == 0) {
       schedule_rr();
    } else if (strncmp(argv[1], "sjf", 3) == 0) {
       schedule_sjf();
    } else {
       printf("Error: scheduler '%s' not supported \n", argv[1]);
       return -1;
    }

    return 0;
}
