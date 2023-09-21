#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    /*for a child process */
    pid = fork();

    if(pid < 0) {
    }
    else if(pid == 0) {
        printf("Child Process");
        printf("Pid %d\n", getpid()); 
    }
    else {
         wait(NULL);
         printf("Parent Process");
         printf("Pid: %d\n", getpid());
    }

    return 0;
}