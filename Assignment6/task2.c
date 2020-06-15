#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "myqueue.h"

#define THREAD_COUNT 5
#define ZEROS_COUNT 5
#define ONES_COUNT 10000

pthread_mutex_t thread_mutex;


void *consumers(void *arg) {
	unsigned int sum = 0;
	unsigned int ret = 1;

	while (ret != 0) {
		// Lock while using queue
		pthread_mutex_lock(&thread_mutex);

		// do not use front when queue is empty (queue.front gives 0 back when isempty)
		if (!empty()) {
			ret = front();
			pop();
			sum += ret;
		}
		// Unlock for other threads
		pthread_mutex_unlock(&thread_mutex);

	}
	printf("%d\n", sum);
	return NULL;
}

void producer() {
	// Fill queue
	// lock while read/write and unlock after read/write
	for (int i = 0; i < ONES_COUNT; ++i) {
		pthread_mutex_lock(&thread_mutex);
		push(1);
		pthread_mutex_unlock(&thread_mutex);
	}
	for (int i = 0; i < ZEROS_COUNT; ++i) {
		pthread_mutex_lock(&thread_mutex);
		push(0);
		pthread_mutex_unlock(&thread_mutex);
	}
}


int main(void)
{
	int ret[THREAD_COUNT];
	pthread_t thread[THREAD_COUNT];


	pthread_mutex_init(&thread_mutex, NULL);

	// create queue
	create();


	// create 5 Threads
	for (int i = 0; i < THREAD_COUNT; i++) {
		ret[i] = pthread_create(&thread[i], NULL, consumers, NULL);
		if (ret[i] != 0)
		{
			fprintf(stderr, "ERROR %d: Cant create thread\n", ret[i]);
			return EXIT_FAILURE;
		}
	}

	// Start to produce
	producer();


	// Wait for finish
	for (int i = 0; i < THREAD_COUNT; i++) {
		ret[i] = pthread_join(thread[i], NULL);
		if (ret[i] != 0)
		{
			fprintf(stderr, "ERROR %d: Cant join thread\n", ret[i]);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}