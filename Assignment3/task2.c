#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int done = 0;

void handler(int sig) {
    if (sig == SIGCHLD) {
        done = 1;
        printf("Child exited.\n");
    }
}

int main(void) {
    signal(SIGCHLD, handler);

    // Child process
    if (fork() == 0) {
        printf("[Child] pid %d \n", getpid());
        _exit(0);
    }

    while (!done) {
        continue;
    }

    return EXIT_SUCCESS;
}