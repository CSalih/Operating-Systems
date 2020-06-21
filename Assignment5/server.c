#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SEM_KEY 9979
#define SHM_KEY 9979

int main(void) {
    int shmid, semid, buf, RESULT_FIFO;
    char *fifo = "/tmp/RESULT_FIFO";
    fd_set readCheck;

    // Create a Sem
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0644);
    if (semid < 0) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Allocate a SHM
    shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0660);  // IPC_EXCL
    if (shmid < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach a SHM
    int *data = (int *)shmat(shmid, NULL, 0);  // SHM_WRONLY
    if (*data == -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Write to the SHM
    *data = 0;

    // remove if exists
    unlink(fifo);

    // Create named FIFO
    if (mkfifo(fifo, 0666) == -1) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    // Open FIFO
    RESULT_FIFO = open(fifo, O_RDONLY);

    // Read from FIFO and print the content to stdout
    FD_ZERO(&readCheck);
    FD_SET(RESULT_FIFO, &readCheck);

    if (select(RESULT_FIFO + 1, &readCheck, NULL, NULL, NULL) < 0) {
        perror("select");
        return EXIT_SUCCESS;
    }

    // Read only when a msg is avalible
    if (FD_ISSET(RESULT_FIFO, &readCheck)) {
        if (read(RESULT_FIFO, &buf, sizeof(buf)) != -1)
            fprintf(stdout, "Received %d from RESULT_FIFO\n", buf);
    }

    // Close File Descriptor and remove fifo
    close(RESULT_FIFO);
    unlink(fifo);

    // remove SHM
    if (shmctl(shmid, IPC_RMID, NULL) < 0) perror("shmctl");

    if (semctl(semid, 0, IPC_RMID) < 0) perror("semctl");

    return EXIT_SUCCESS;
}
