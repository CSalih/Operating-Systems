#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "best_fit_allocator.h"

typedef struct Alloc {
    int size;
    int count;
} Alloc;

pthread_mutex_t mutex;

void *doMalloc(void *arg) {
    Alloc *allocData = (Alloc *)arg;

    for (int i = 0; i < allocData->count; i++) {
        // Lock while using queue
        pthread_mutex_lock(&mutex);

        char *tmp = (char *)myMalloc(allocData->size);
        if (tmp == NULL) {
            printf("Malloc failed, no space available\n");
            return NULL;
        }
        myFree(tmp);

        // Unlock for other threads
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
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

    initAllocator();

    // init mutex
    pthread_mutex_init(&mutex, NULL);

    // create Threads
    for (int i = 0; i < threadCount; i++) {
        ret = pthread_create(&thread[i], NULL, doMalloc, &allocData);
        if (ret != 0) {
            fprintf(stderr, "ERROR %d: Cant create thread\n", ret);
            return EXIT_FAILURE;
        }
    }

    // Wait for finish
    for (int i = 0; i < threadCount; i++) {
        ret = pthread_join(thread[i], NULL);
        if (ret != 0) {
            fprintf(stderr, "ERROR %d: Cant join thread\n", ret);
            return EXIT_FAILURE;
        }
    }

    // remove mutex
    pthread_mutex_destroy(&mutex);

    destroyAllocator();
    return EXIT_SUCCESS;
}