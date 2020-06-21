#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define PIPE_BUF 4096
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void printSelect(int fdWS, int fdMW, int fdDB) {
    fd_set rfds;
    int res;
    char buf[PIPE_BUF];

    /* Initialize the file descriptor set. */
    FD_ZERO(&rfds);
    FD_SET(fdWS, &rfds);
    FD_SET(fdMW, &rfds);
    FD_SET(fdDB, &rfds);

    // highest fifo
    int maxfd = MAX(fdWS, MAX(fdMW, fdDB));

    // wait for a IO operation
    if (select(maxfd + 1, &rfds, NULL, NULL, NULL) == -1) {
        perror("select");
    }

    if (FD_ISSET(fdWS, &rfds)) {
        res = read(fdWS, buf, sizeof(buf));
        if (res > 0) printf("Received: %s from [web]\n", buf);
    }
    if (FD_ISSET(fdMW, &rfds)) {
        res = read(fdMW, buf, sizeof(buf));
        if (res > 0) printf("Received: %s from [middle-ware]\n", buf);
    }
    if (FD_ISSET(fdDB, &rfds)) {
        res = read(fdDB, buf, sizeof(buf));
        if (res > 0) printf("Received: %s from [database]\n", buf);
    }
}

int main(void) {
    pid_t pid;

    int dbServerFd, mwServerFd, wsServerFd;
    char *fifoDb = "/tmp/ex04_db";
    char *fifoMw = "/tmp/ex04_mw";
    char *fifoWs = "/tmp/ex04_ws";

    // remove file
    unlink(fifoDb);
    unlink(fifoMw);
    unlink(fifoWs);

    // create FIFO's
    mkfifo(fifoDb, 0666);
    mkfifo(fifoMw, 0666);
    mkfifo(fifoWs, 0666);

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

        // create fifo file
        dbServerFd = open(fifoDb, O_WRONLY);
        mwServerFd = open(fifoMw, O_WRONLY);
        wsServerFd = open(fifoWs, O_WRONLY);

        while (1) {
            char *buf;

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
                    snprintf(buf, bufsz + 1, "message %d", counterDb++);
                    write(dbServerFd, buf, sizeof(buf));
                    break;
                case 1:;  // bypass: a label can only be part of a statement and
                          // a declaration is not a statement
                    bufsz = snprintf(NULL, 0, "message %d", counterMw);
                    buf = malloc(bufsz + 1);
                    snprintf(buf, bufsz + 1, "message %d", counterMw++);
                    write(mwServerFd, buf, sizeof(buf));
                    break;
                case 2:;  // bypass: a label can only be part of a statement and
                          // a declaration is not a statement
                    bufsz = snprintf(NULL, 0, "message %d", counterWs);
                    buf = malloc(bufsz + 1);
                    snprintf(buf, bufsz + 1, "message %d", counterWs++);
                    write(wsServerFd, buf, sizeof(buf));
                    break;
            }
            sleep(randNum);
            free(buf);
        }

        // close file
        close(dbServerFd);
        close(mwServerFd);
        close(wsServerFd);

        // delete file
        unlink(fifoDb);
        unlink(fifoMw);
        unlink(fifoWs);

        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        // read fifo
        if ((dbServerFd = open(fifoDb, O_RDONLY)) < 0)
            perror("ERRROR: Cant open fifo.\n");
        if ((mwServerFd = open(fifoMw, O_RDONLY)) < 0)
            perror("ERRROR: Cant open fifo.\n");
        if ((wsServerFd = open(fifoWs, O_RDONLY)) < 0)
            perror("ERRROR: Cant open fifo.\n");

        while (1) {
            printSelect(wsServerFd, mwServerFd, dbServerFd);
        }

        // wait for child terminate
        if ((waitpid(pid, NULL, 0)) < 0) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        // close file
        close(dbServerFd);
        close(mwServerFd);
        close(wsServerFd);

        // delete file
        unlink(fifoDb);
        unlink(fifoMw);
        unlink(fifoWs);
    }

    return EXIT_SUCCESS;
}
