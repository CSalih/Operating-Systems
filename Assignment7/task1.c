#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "myqueue.h"

#define THREAD_COUNT 5
#define ZEROS_COUNT 5
#define ONES_COUNT 10000

pthread_spinlock_t thread_locker;

void *consumers(void *arg) {
    unsigned int sum = 0;
    unsigned int ret = 1;

    while (ret != 0) {
        // Lock while using queue
        pthread_spin_lock(&thread_locker);

        // do not use front when queue is empty (queue.front gives 0 back when
        // isempty)
        if (!empty()) {
            ret = front();
            pop();
            sum += ret;
        }
        // Unlock for other threads
        pthread_spin_unlock(&thread_locker);
    }
    printf("%d\n", sum);
    return NULL;
}

void producer() {
    // Fill queue
    // lock while read/write and unlock after read/write
    for (int i = 0; i < ONES_COUNT; ++i) {
        pthread_spin_lock(&thread_locker);
        push(1);
        pthread_spin_unlock(&thread_locker);
    }
    for (int i = 0; i < ZEROS_COUNT; ++i) {
        pthread_spin_lock(&thread_locker);
        push(0);
        pthread_spin_unlock(&thread_locker);
    }
}

int main(void) {
    int ret[THREAD_COUNT];
    pthread_t thread[THREAD_COUNT];

    // init spin
    pthread_spin_init(&thread_locker, PTHREAD_PROCESS_SHARED);

    // create queue
    create();

    // create 5 Threads
    for (int i = 0; i < THREAD_COUNT; i++) {
        ret[i] = pthread_create(&thread[i], NULL, consumers, NULL);
        if (ret[i] != 0) {
            fprintf(stderr, "ERROR %d: Cant create thread\n", ret[i]);
            return EXIT_FAILURE;
        }
    }

    // Start to produce
    producer();

    // Wait for finish
    for (int i = 0; i < THREAD_COUNT; i++) {
        ret[i] = pthread_join(thread[i], NULL);
        if (ret[i] != 0) {
            fprintf(stderr, "ERROR %d: Cant join thread\n", ret[i]);
            return EXIT_FAILURE;
        }
    }

    // remove spin
    pthread_spin_destroy(&thread_locker);

    return EXIT_SUCCESS;
}