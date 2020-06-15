#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

// This is a silly glibc bug
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#define THREAD_COUNT 10

void *writeToFile(void *threadID);

int main(int argc, char** argv)
{
	int ret[THREAD_COUNT];
	pthread_t thread[THREAD_COUNT];

	// seed rand
	srand((unsigned int)time(NULL));

	// Create THREAD_COUNT threads
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		ret[i] = pthread_create(&thread[i], NULL, writeToFile, &i);
		if (ret[i] != 0)
		{
			fprintf(stderr, "ERROR %d: Cant create thread\n", ret[i]);
			return EXIT_FAILURE;
		}
	}

	// Close a thread with a chance of 50%
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		if (rand() % 2)
			pthread_cancel(thread[i]);
	}


	// Wait for finish
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		ret[i] = pthread_join(thread[i], NULL);
		if (ret[i] != 0)
		{
			fprintf(stderr, "ERROR %d: Cant join thread\n", ret[i]);
			return EXIT_FAILURE;
		}
	}

	return(EXIT_SUCCESS);
}

void cleanupHandler(void *arg)
{
	// close file descriptor
	FILE *fp = (FILE*)arg;
	if (fp != NULL)
	{
		if (fclose(fp))
			perror("cleanuphander");
	}
}


void *writeToFile(void *threadID)
{
	int waitTime = 0;
	char filename[12];
	FILE* fptr = NULL;


	//Sleep between 0 - 3 Seconds
	waitTime = rand() % 4;
	sleep(waitTime);

	//format filename
	sprintf(filename, "Thread%d.txt", *(int*)threadID);


	/*	will called when thread is canceled, calls pthread_exit or
	pthread_cleanup_pop is executed with non-zero EXECUTE argument */
	pthread_cleanup_push(cleanupHandler, fptr);

	// try to open a file
	fptr = fopen(filename, "rb+");
	if (fptr == NULL)
	{
		// file does not exist, create it
		fptr = fopen(filename, "wb");
		if (fptr == NULL)
		{
			printf("Disc full or no permission\n");
			return NULL;
		}
	}


	// Write file
	fprintf(fptr, "%d\n", (int)gettid());

	// call cleanup handler
	pthread_cleanup_pop(1);

	// close
	fclose(fptr);

	return NULL;
}