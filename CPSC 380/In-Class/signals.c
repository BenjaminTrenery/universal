#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define WORK_SIZE 1024
char work_area[WORK_SIZE];


void *thread_function(void *arg) {

        while (strncmp("end", work_area, 3) != 0) {
            printf("You input %d chars \n", strlen(work_area) - 1);
        }

    pthread_exit(NULL);
}

int main()
{
    int res;
    pthread_t tid;
    void *thread_result;
    res = pthread_create(&tid, NULL, thread_function, NULL);

    if (res != 0) {
        printf("Error thread create failed, errno = %d (%s) \n", errno,
        strerror(errno));
        return -1;
    }

    printf("Input some text. Enter 'end' to finish \n");

    while (strncmp("end", work_area, 3) != 0) {
        fgets(work_area, WORK_SIZE, stdin);
    }

    printf("Waiting for thread '%d' to finish... \n", tid);
    res = pthread_join(tid, &thread_result);

    if (res != 0) {
        printf("Error thread join failed, errno = %s (%s) \n", errno,
        strerror(errno));
        return -1;
    }

    printf("Thread '%d' joined \n", tid);
    return 0;
}