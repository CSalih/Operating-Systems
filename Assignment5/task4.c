#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#define SHM_KEY 9979

int main(void)
{
	// n -> Process counter, m = loops each process
	const int n = 1000, m = 10000;
	int pid, shmid, semid, RESULT_FIFO;
	char *fifo = "/tmp/RESULT_FIFO";
	sem_t mutex;

	// get SEM
	semid = sem_init(&mutex, 0, 1);
	if (semid < 0) {
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	// Get SHM
	shmid = shmget(SHM_KEY, 0, 0660);
	if (shmid < 0) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	int *data = (int *)shmat(shmid, NULL, 0); //SHM_WRONLY
	if (*data == -1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	// Creates n Processes
	for (int i = 1; i <= n; i++) {
		// Create fork
		pid = fork();
		if (pid == 0)
		{
			for (int i = 1; i <= m; i++) {
				/* Join critical area */
				sem_wait(&mutex);

				// Inc val in SHM
				*data = *data + 1;

				/* Leave critical area */
				sem_post(&mutex);
			}

			exit(EXIT_SUCCESS);
		}
	}

	// Wait for Child
	for (int i = 1; i <= n; i++)
		waitpid(pid, NULL, 0);

	// Read from SHM > stdout
	fprintf(stdout, "Data: %d\n", *data);

	//Write to RESULT_FIFO
	RESULT_FIFO = open(fifo, O_WRONLY);
	if (RESULT_FIFO != -1)
		write(RESULT_FIFO, data, sizeof(data));

	// destroy mutex
	sem_destroy(&mutex);

	// close fifo
	close(RESULT_FIFO);

	return EXIT_SUCCESS ;
}
