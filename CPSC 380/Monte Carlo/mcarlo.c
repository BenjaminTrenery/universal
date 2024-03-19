#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_THREADS 2 /* number of threads*/
double hit_count; /*this data is shared by the thread(s)*/
void *runner(void *param); /*threads call this function*/

/* Generates a double precision random number */
double random_double()
{
    return random() / ((double)RAND_MAX + 1);
}

/* the thread will begin control in this function*/
void *runner(void *param) {

    int npoints = atoi(param) / NUM_THREADS;
    /* Check for points inside circle */
    for (int i = 0; i < npoints; i++) {
        /* generate random numbers between -1.0 and +1.0 (exclusive) */
        double x = random_double() * 2.0 - 1.0;
        double y = random_double() * 2.0 - 1.0;
        if (sqrt(x*x + y*y) < 1.0 )
            ++hit_count;
    }

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    
    pthread_t workers[NUM_THREADS]; /*an array of threads to be joined*/
    pthread_attr_t attr; /*set of thread attributes*/

    if(argc != 2) {
        fprintf(stderr, "usage: a.out <integer value>\n)");
        return -1;
    }

    int totalNpoints = atoi(argv[1]); 

    if(totalNpoints < 0 || totalNpoints % NUM_THREADS != 0) {
        fprintf(stderr, "%d must be >= 0 and divisible by %d\n", atoi(argv[1]), NUM_THREADS);
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

    double pi = (4 * (hit_count)) / totalNpoints;

    printf("π = %f\n", pi);

    return 0;
}

