#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void signalHandler(int sig)
{
	switch (sig)
	{
	SIGUSR1:
		printf("Hello from child!\n");
		break;
	SIGUSR2:
		break;

	default:
		break;
	}
}

int main(void)
{

	pid_t pid = fork();

	if (pid == 0)
	{
		// Child process
		printf("[Child] pid %d \n", getpid());

		signal(SIGUSR1, NULL);
		sleep(13);

		signal(SIGUSR1, signalHandler);

		signal(SIGUSR2, signalHandler);
		sleep(3);

		_exit(EXIT_SUCCESS);
	}
	else if (pid > 0)
	{
		// Parent process
		sleep(5);
		kill(pid, SIGUSR2);
		alarm(13);

		while (1)
		{
			// Break if child terminated
			if (wait(NULL))
				break;

			sleep(5);
			kill(pid, SIGUSR1);
		}
	}

	return EXIT_SUCCESS;
}