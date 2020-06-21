#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// structure for message queue
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main(void) {
    pid_t pid;
    key_t keyDb;
    key_t keyMw;
    key_t keyWs;

    int idDb;
    int idMw;
    int idWs;

    // init rand
    srand((unsigned int)time(NULL));

    // create fork
    pid = fork();

    if (pid == 0) {
        /* Child process closes up input side of pipe */
        int randNum, randServer;
        int bufsz;

        int counterDb = 1;
        int counterMw = 1;
        int counterWs = 1;

        // genatate unique key
        keyDb = ftok("database", 60);
        keyMw = ftok("middleware", 61);
        keyWs = ftok("webserver", 62);

        // msgget creates a message queue
        idDb = msgget(keyDb, 0666 | IPC_CREAT);
        idMw = msgget(keyMw, 0666 | IPC_CREAT);
        idWs = msgget(keyWs, 0666 | IPC_CREAT);

        if (idDb == -1 || idMw == -1 || idWs == -1) perror("msgget");

        while (1) {
            char* buf;

            // get random time between 0 and 2 seconds
            randServer = rand() % 3;
            // get random time between 2 and 4 seconds
            randNum = rand() % 5 + 2;

            // write message
            switch (randServer) {
                case 0:;  // bypass: a label can only be part of a statement and
                          // a declaration is not a statement
                    bufsz = snprintf(NULL, 0, "message %d", counterDb);
                    buf = malloc(bufsz + 1);

                    message.msg_type = 1;
                    snprintf(message.msg_text, bufsz + 1, "message %d",
                             counterDb++);
                    if (msgsnd(idDb, &message, sizeof(message), 0) == -1)
                        perror("msgsnd");

                    break;
                case 1:;  // bypass: a label can only be part of a statement and
                          // a declaration is not a statement
                    bufsz = snprintf(NULL, 0, "message %d", counterMw);
                    buf = malloc(bufsz + 1);

                    message.msg_type = 1;
                    snprintf(message.msg_text, bufsz + 1, "message %d",
                             counterMw++);
                    if (msgsnd(idMw, &message, sizeof(message), 0))
                        perror("msgsnd");

                    break;
                case 2:;  // bypass: a label can only be part of a statement and
                          // a declaration is not a statement
                    bufsz = snprintf(NULL, 0, "message %d", counterWs);
                    buf = malloc(bufsz + 1);

                    message.msg_type = 1;
                    snprintf(message.msg_text, bufsz + 1, "message %d",
                             counterWs++);
                    if (msgsnd(idWs, &message, sizeof(message), 0))
                        perror("msgsnd");
                    break;
            }
            sleep(randNum);
            free(buf);
        }

        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        // genatate uniqe key
        keyDb = ftok("database", 60);
        keyMw = ftok("middleware", 61);
        keyWs = ftok("webserver", 62);

        // msgget creates a message queue
        idDb = msgget(keyDb, 0666 | IPC_CREAT);
        idMw = msgget(keyMw, 0666 | IPC_CREAT);
        idWs = msgget(keyWs, 0666 | IPC_CREAT);

        if (idDb == -1 || idMw == -1 || idWs == -1) perror("msgget");

        // display the message
        while (1) {
            // msgrcv to receive message
            if (msgrcv(idDb, &message, sizeof(message), 1, 0) != -1) {
                printf("Data Received is : %s \n", message.msg_text);
            }

            if (msgrcv(idMw, &message, sizeof(message), 1, 0) != -1) {
                printf("Data Received is : %s \n", message.msg_text);
            }

            if (msgrcv(idWs, &message, sizeof(message), 1, 0) != -1) {
                printf("Data Received is : %s \n", message.msg_text);
            }
        }

        // wait for child terminate
        if ((waitpid(pid, NULL, 0)) < 0) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        // to destroy the message queue
        msgctl(idDb, IPC_RMID, NULL);
        msgctl(idMw, IPC_RMID, NULL);
        msgctl(idWs, IPC_RMID, NULL);
    }

    return EXIT_SUCCESS;
}
