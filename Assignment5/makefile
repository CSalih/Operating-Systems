CC = gcc
LD = gcc

CFLAGS = -std=gnu99 -Wall -Werror -Wextra

.PHONY: all task1 task2 task4 clean

all: server task1 task2 task4
task1: server
task2: server
task4: server

clean:
	$(RM) server task1 task2 task4

server: server.c
	$(CC) $(CFLAGS) -o $@ $<
task1: task1.c
	$(CC) $(CFLAGS) -o $@ $<
task2: task2.c
	$(CC) $(CFLAGS) -o $@ $<
task4: task4.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread -lrt