#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

int value = 7;
int main() {
        
    pid_t pid = fork();
    printf("%d\n", pid);
        
    if (pid == 0) {
        value -= 5;
        return 0;
    }
    else if (pid > 0) {
        wait(NULL);
        printf("Parent: value = %d \n", value);
        return 0;
    }
}
