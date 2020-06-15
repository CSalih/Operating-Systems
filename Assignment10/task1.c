#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h> 

#include "best_fit_allocator.h"

typedef struct Alloc {
	int size;
	int count;
} Alloc;

pthread_mutex_t mutex;


void *doMalloc(void *arg) {

	Alloc *allocData = (Alloc*)arg;

	unsigned int seed = time(NULL);

	// Perform N allocations and allocate another N/2 blocks of random size.
	for (int i = 0; i < allocData->count * 1.5; i++)
	{
		// perform N allocations of random sizes between S and 8*S
		int size = (rand_r(&seed) % ((8 * allocData->size ) - allocData->size)) + allocData->size;

		// Lock while using queue
		pthread_mutex_lock(&mutex);

		char *tmp = (char*)myMalloc(size);
		if (tmp == NULL) {
			printf("Malloc failed, no space available\n");
			destroyAllocator();
			exit(-1);
		}
		// randomly(with a 50 % chance)
		if (rand_r(&seed) % 2) {
			if (tmp != NULL) {
				myFree(tmp);
			}
		}

		// Unlock for other threads
		pthread_mutex_unlock(&mutex);
	}


	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Usage: ./membench 8 10000 1024\n");
		return EXIT_FAILURE;
	}

	Alloc allocData;
	int threadCount = atoi(argv[1]);
	allocData.count = atoi(argv[2]);
	allocData.size = atoi(argv[3]);

	int ret;
	pthread_t thread[threadCount];

	initAllocator();

	// init mutex
	pthread_mutex_init(&mutex, NULL);


	// create Threads
	for (int i = 0; i < threadCount; i++) {
		ret = pthread_create(&thread[i], NULL, doMalloc, &allocData);
		if (ret != 0)
		{
			fprintf(stderr, "ERROR %d: Cant create thread\n", ret);
			return EXIT_FAILURE;
		}
	}

	// Wait for finish
	for (int i = 0; i < threadCount; i++) {
		ret = pthread_join(thread[i], NULL);
		if (ret != 0)
		{
			fprintf(stderr, "ERROR %d: Cant join thread\n", ret);
			return EXIT_FAILURE;
		}
	}

	// remove mutex
	pthread_mutex_destroy(&mutex);


	destroyAllocator();
	return EXIT_SUCCESS;
}