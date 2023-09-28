#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80 /* The maximum length command*/

int main(void) {

    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program*/
    int inputSize = 100;

    pid_t pid;

    while (should_run) {

        int argsSize = -1;
        printf("osh> ");
        fflush(stdout);

        char input[inputSize];

        scanf("%s", input);

        for(int i = 0; i < inputSize; i++) {
            char *token = strtok(input, " ");

            if(token != NULL){
                args[i] = token;
            }
            else {

                argsSize = i - 1;
                break;
            }
        }

        // pid = fork();

        // if(pid < 0) {
        //     exit(0);
        // }
        // else if (pid == 0){

        //     execvp(args[0], args);
        //     exit(0);
        // }
        // else {

        //     if(args[argsSize][0] != '&') {
        //         wait(NULL);
        //     }
        //     else {
        //         exit(0);
        //     }
        // }

        printf("%s\n", input);
        
    }

    return 0;
}