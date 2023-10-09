#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2
int sum; /*this data is shared by the thread(s)*/
void *runner(void *param); /*threads call this function*/

/* the thread will begin control in this function*/
void *runner(void *param) {
    int i, upper = atoi(param);
    sum = 0;

    for(i = 1; i <= upper; i++) {
        sum += i;
    }

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    /*an array of threads to be joined*/
    pthread_t workers[NUM_THREADS];
    pthread_attr_t attr; /*set of thread attributes*/

    if(argc != 2) {
        fprintf(stderr, "usage: a.out <integer value>\n)");
        return -1;
    }

    if(atoi(argv[1]) < 0) {
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }

    /*get the default attributes */
    pthread_attr_init(&attr);
    /*create the thread */
    for(int x = 0; x < NUM_THREADS; x++) {
        pthread_create(&workers[x],&attr, runner, argv[1]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }

    printf("sum = %d\n", sum);

    return 0;
}

