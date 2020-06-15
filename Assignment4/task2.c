#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char **argv)
{
	int fd[2];
	int pid;
	int res;


	if (argc != 2)
	{
		fprintf(stderr, "ERROR: Worng arguments!\n");
		return EXIT_FAILURE;
	}

	// Args for execvp
	char *grep[] = { "grep", argv[1], NULL };
	char *ls[] = { "ls", NULL };
	
	// create pipe
	pipe(fd);

	// create fork
	pid = fork();
	if (pid == 0) {
		// Child

		// Change stdout to pipe[1] and close fd read
		dup2(fd[1], 1);
		close(fd[0]);

		// run ls
		res = execvp("ls", ls);

		// close fd write
		close(fd[1]);
		exit(res);
	}
	else if (pid > 0)
	{
		// Parent: 
		//Input is from pipe[0] and output is via stdout.
		dup2(fd[0], 0);
		close(fd[1]);

		// run grep
		res = execvp("grep", grep);
		close(fd[0]);
	}

	return EXIT_SUCCESS;
}