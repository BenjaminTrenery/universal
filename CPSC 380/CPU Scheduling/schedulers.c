#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "schedulers.h"

//used to free the memory of the myTasks array and put at the end of scheduler functions
void freeMem(){

    for(int i = 0; i < TASK_SIZE; i++) {

        free(myTasks[i]);
    }
}

//creates task struct from parameters and stores in myTasks array
void add(char *name, int priority, int burst) {

    Task *myTask = malloc(sizeof(Task));
    myTask->name = name;
    myTask->priority = priority;
    myTask->burst = burst;
    myTask->tid = tidIDNum;

    myTasks[currentSize] = myTask;

    tidIDNum++;
    currentSize++;
}

//first come first server scheduler (goes in order of items in file)
void schedule_fcfs() {

    for(int i = 0; i < currentSize; i++) {
        
        Task *myTask = myTasks[i];

        int burst = myTask->burst;
        int currentSlice = 1;

        while(currentSlice <= burst) {
            run(myTask, currentSlice);

            sleep(1);

            currentSlice++;
        }

    }

    freeMem();
}

//priority scheduler, rearranges myTasks array from smallest priority values to largest and then runs the tasks
void schedule_priority() {

    for(int i = 0; i < currentSize; i++){ 
        
        Task *currentTask = myTasks[i];

        int currentPriority = currentTask->priority;
        // printf("Got here 1\n");

        int x = i - 1;

        while(x >= 0 && currentPriority < myTasks[x]->priority) {
            myTasks[x + 1] = myTasks[x];
            x--;
        }

        myTasks[x + 1] = currentTask;

    }

    for(int n = 0; n < currentSize; n++) {
        
        Task *myTask = myTasks[n];

        int burst = myTask->burst;
        int currentSlice = 1;

        while(currentSlice <= burst) {
            run(myTask, currentSlice);

            sleep(1);

            currentSlice++;
        }

    }

    freeMem();
}

//round robin scheduler, tasks done in 10 second intervals. When a task's burst is done, the position of the task in myTasks is stored in the finished Pos array 
//each tasks is done randomly until all the bursts are finished
void schedule_rr() {

    time_t t;
    srand((unsigned) time(&t));
    int isFinished = 0;
    int finishedPos[currentSize];
    int currentPos = 0;

    while(isFinished == 0) {

        int matchedPos = 0;
        int randomPos = rand() % currentSize;

        for(int i = 0; i < currentSize; i++) {

            if(randomPos == finishedPos[i]) {
                matchedPos = 1;
            }
        }
        
        if(matchedPos == 0) {

            Task *myTask = myTasks[randomPos];
            int burst = myTask->burst;

            int currentSlice = 1;

            while(currentSlice <= burst && currentSlice <= 10) {
                run(myTask, currentSlice);

                sleep(1);

                currentSlice++;
                myTask->burst--;
            }   

            if(myTask->burst <= 0) {
                finishedPos[currentPos] = randomPos;
                currentPos++;
            }
        }

        if(currentPos >= currentSize) {
            isFinished = 1;
        }

    }

    freeMem();
}

//shortest job first scheduler
//rearranges the myTasks array to go from smallest bursts to largest bursts then does does the tasks
void schedule_sjf() {

for(int i = 0; i < currentSize; i++){ 
        
        Task *currentTask = myTasks[i];

        int currentBurst = currentTask->burst;

        int x = i - 1;

        while(x >= 0 && currentBurst < myTasks[x]->burst) {
            myTasks[x + 1] = myTasks[x];
            x--;
        }

        myTasks[x + 1] = currentTask;

    }

    for(int n = 0; n < currentSize; n++) {
        
        Task *myTask = myTasks[n];

        int burst = myTask->burst;
        int currentSlice = 1;

        while(currentSlice <= burst) {
            run(myTask, currentSlice);

            sleep(1);

            currentSlice++;
        }

    }

    freeMem();

}