#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEM_KEY 9979
#define SHM_KEY 9979

int create_semaphore(key_t key) {
    /* Testen, ob das Semaphor bereits existiert */
    int semid = semget(key, 0, IPC_PRIVATE);
    if (semid < 0) {
        /* Semaphor doesnt exists */
        semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
        if (semid < 0) {
            perror("semget");
            return -1;
        }

        /* Init Semaphor with 1 */
        if (semctl(semid, 0, SETVAL, (int)1) == -1) return -1;
    }
    return semid;
}

int main(void) {
    // n -> Process counter, m = loops each process
    const int n = 1000, m = 10000;
    int pid, shmid, semid, RESULT_FIFO;
    char *fifo = "/tmp/RESULT_FIFO";

    // get SEM
    semid = create_semaphore(SEM_KEY);
    if (semid < 0) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

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
            /* Structs for semaphor */
            struct sembuf enter, leave;

            enter.sem_num = leave.sem_num = 0;
            enter.sem_flg = leave.sem_flg = SEM_UNDO;
            enter.sem_op = -1; /* DOWN-Operation */
            leave.sem_op = 1;  /* UP-Operation */

            for (int i = 1; i <= m; i++) {
                /* Join critical area */
                semop(semid, &enter, 1);

                // Inc val in SHM
                *data = *data + 1;

                /* Leave critical area */
                semop(semid, &leave, 1);
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

    semctl(semid, 0, IPC_RMID, 0);

    close(RESULT_FIFO);
    return EXIT_SUCCESS;
}
