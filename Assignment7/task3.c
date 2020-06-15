#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 1000
#define THREAD_ITERATION 10000

#ifdef COMPLILE_TASK3A
	int x = 0;
#else
	#include <stdatomic.h>
	_Atomic int x = 0;
#endif


void *increment(void *arg) {
	for (int i = 0; i < THREAD_ITERATION; ++i) {
		x++;
	}
	return NULL;
}


int main(void) {
	int ret[THREAD_COUNT];
	pthread_t thread[THREAD_COUNT];


	// create 1000 Threads
	for (int i = 0; i < THREAD_COUNT; i++) {
		ret[i] = pthread_create(&thread[i], NULL, increment, NULL);
		if (ret[i] != 0) {
			fprintf(stderr, "ERROR %d: Cant create thread\n", ret[i]);
			return EXIT_FAILURE;
		}
	}

	// Wait for finish
	for (int i = 0; i < THREAD_COUNT; i++) {
		ret[i] = pthread_join(thread[i], NULL);
		if (ret[i] != 0) {
			fprintf(stderr, "ERROR %d: Cant join thread\n", ret[i]);
			return EXIT_FAILURE;
		}
	}


	printf("Result: %d\n", x);

	return EXIT_SUCCESS;
}