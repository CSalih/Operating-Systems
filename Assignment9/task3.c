#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define USE_LOCAL_ALLOCATOR
#include "best_fit_allocator.h"



typedef struct Alloc {
	int size;
	int count;
} Alloc;


void *doMalloc(void *arg) {

	Alloc *allocData = (Alloc*)arg;
	initAllocator();
	for (int i = 0; i < allocData->count; i++)
	{
		// Lock while using queue
		char *tmp = (char*)myMalloc(allocData->size);
		if (tmp == NULL) {
			printf("Malloc failed, no space available\n");
			return NULL;
		}
		myFree(tmp);

	}
	destroyAllocator();

	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Usage: ./membench 8 10000 1024");
		return EXIT_FAILURE;
	}

	Alloc allocData;
	int threadCount = atoi(argv[1]);
	allocData.count = atoi(argv[2]);
	allocData.size = atoi(argv[3]);

	int ret;
	pthread_t thread[threadCount];



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

	return EXIT_SUCCESS;
}