/* Compile with
 * gcc -Wall -Werror  exam.c -std=gnu99 -pthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_spinlock_t spin_lock;
const int COUNT_MSG = 5;
const int THREAD_COUNT = 3;

typedef struct
{
	int ticket;
	int hasLog;
	char *msg;
} logMsgStruct;

logMsgStruct logMsg;

void logAMessage()
{
	// seed rand with the current time with a cast to an unsigned integer
	srand((unsigned int)time(NULL));

	// rand
	int randNum = rand() % 3;

	// get rand server
	switch (randNum)
	{
	case 0:
		logMsg.msg = "web_server";
		break;
	case 1:
		logMsg.msg = "database";
		break;
	case 2:
		logMsg.msg = "weather_station";
		break;
		// default not need
	}
}

// Service Thread
void *serviceThread(void *arg)
{
	for (int i = 0; i < COUNT_MSG; i++)
	{
		// Enter Critical Zone
		pthread_spin_lock(&spin_lock);

		// if hasLog is false we can log
		if (!logMsg.hasLog)
		{
			logAMessage();

			// set hasLog to true
			logMsg.hasLog = 1;
		}
		else
		{
			i--;
		}

		// Leave Critical Zone
		pthread_spin_unlock(&spin_lock);
	}

	return EXIT_SUCCESS;
}

void *loggerThread(void *arg)
{
	// compute the message counts
	int count = COUNT_MSG * THREAD_COUNT;

	// We need in a while(ticket < 15) loop a spin_lock,
	// so i use a for loop  and decrement when there is no log
	for (int i = 0; i < count; i++)
	{
		// Enter Critical Zone
		pthread_spin_lock(&spin_lock);

		if (logMsg.hasLog)
		{
			// print log message (Glob var beginnt mit 0 -> n + 1)
			printf("%d %s\n", logMsg.ticket + 1, logMsg.msg);

			// inc ticket
			logMsg.ticket = logMsg.ticket + 1;

			// set hasLog to false
			logMsg.hasLog = 0;
		}
		else
		{
			// dekrement
			i--;
		}

		// Leave Critical Zone
		pthread_spin_unlock(&spin_lock);
	}

	// exit
	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	int ret;						   // return val of the pthread functions
	pthread_t logThread[THREAD_COUNT]; // Thread for Service
	pthread_t readerThread;			   // Thread for Logger

	// init Spin
	pthread_spin_init(&spin_lock, PTHREAD_PROCESS_SHARED);

	// Create a Service Thread
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		// create logThread with the function serviceThread and give no args
		ret = pthread_create(&logThread[i], NULL, serviceThread, NULL);
		// if ret == 0 then there is an error
		if (ret != 0)
		{
			fprintf(stderr, "ERROR %d: Cant create thread\n", ret);
			return EXIT_FAILURE;
		}
	}

	// Create a Logging Thread
	ret = pthread_create(&readerThread, NULL, loggerThread, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "ERROR %d: Cant create thread\n", ret);
		return EXIT_FAILURE;
	}

	// Wait for finish (Service Thread)
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		ret = pthread_join(logThread[i], NULL);
		if (ret != 0)
		{
			fprintf(stderr, "ERROR %d: Cant join thread\n", ret);
			return EXIT_FAILURE;
		}
	}

	// Wait for finish (Reader Thread)
	ret = pthread_join(readerThread, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "ERROR %d: Cant join thread\n", ret);
		return EXIT_FAILURE;
	}

	// destroy spin
	pthread_spin_destroy(&spin_lock);

	return EXIT_SUCCESS;
}
