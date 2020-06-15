#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>



void taskA() {
	int n = 7;

	for (int i = 0; i < n; i++) {
		if (fork() == 0)
			exit(0);
	}


	for (int i = 0; i < n; i++) {
		wait(NULL);
	}
}

void taskB() {
	int n = 12;


	for (int i = 0; i < n; i++) {
		if (fork() == 0) {
			printf("[Child] pid %d \n", getpid());
			exit(0);
		}
	}

	for (int i = 0; i < n; i++) {
		wait(NULL);
	}

	printf("%d child processes have been created\n", n);
}


int main(void) {
	taskA();
	taskB();

	return EXIT_SUCCESS;
}