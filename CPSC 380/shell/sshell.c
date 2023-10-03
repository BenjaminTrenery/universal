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
    const char *s = " "; /* delimiter for strtok token*/

    while (should_run) {

        int i = 0; /* keeps amount of arguments in input*/
        printf("osh> ");
        fflush(stdout);
        char input[100]; /* char array where user input is stored*/
        char *token; 

        //gets user input and gets rid of all characters not used in array
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';

        //stops segmentation fault from empty input
        if(strlen(input) > 0) {

            //token function that finds all the arguments in the input and puts them into args array
            token = strtok(input, s);

            while(token != NULL) {

                args[i] = token;

                i++;
                token = strtok(NULL, s);
            }

            //changes last argument to NULL so that execvp function knows when command ends
            args[i] = NULL; 

            //checks whether the function has a '&' and then waits if there isn't one
            if(args[(i - 1)][0] != '&') {
                    wait(NULL);
                }
            else {
                args[i - 1] = NULL;
            }

            //creates fork process where the child will perform shell commands based on inputted commands from user
            pid = fork();

            if(pid < 0) {
                exit(0);
            }
            else if (pid == 0){

                execvp(args[0], args);
                exit(0);
            }

            //checks if the input was exit and then ends the loop if it was to end the program
            if(strcmp(args[0], "exit") == 0) {
                should_run = 0;
            }

        }
 
    }

    return 0;
}