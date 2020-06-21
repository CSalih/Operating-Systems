﻿#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_KEY 9979

int main(void) {
    // n -> Process counter, m = loops each process
    const int n = 1000, m = 10000;
    int pid, shmid, RESULT_FIFO;
    char *fifo = "/tmp/RESULT_FIFO";

    // Get SHM
    shmid = shmget(SHM_KEY, 0, 0660);
    if (shmid < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    int *data = (int *)shmat(shmid, NULL, 0);  // SHM_WRONLY
    if (*data == -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Creates n Processes
    for (int i = 1; i <= n; i++) {
        // Create fork
        pid = fork();
        if (pid == 0) {
            for (int i = 1; i <= m; i++) {
                // Inc val in SHM
                *data = *data + 1;
            }

            exit(EXIT_SUCCESS);
        }
    }

    // Wait for Child
    for (int i = 1; i <= n; i++) waitpid(pid, NULL, 0);

    // Read from SHM > stdout
    fprintf(stdout, "Data: %d\n", *data);

    // Write to RESULT_FIFO
    RESULT_FIFO = open(fifo, O_WRONLY);
    if (RESULT_FIFO != -1) write(RESULT_FIFO, data, sizeof(data));

    close(RESULT_FIFO);
    return EXIT_SUCCESS;
}
