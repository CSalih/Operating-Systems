CC = gcc
LD = gcc

CFLAGS = -std=gnu11 -Wall -Werror
LDFLAGS= -pthread


.PHONY: all task1 task2 task3 clean

all: task1 task2 task3


clean:
	$(RM) task1 task2 task3


task1: task1.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
task2: task2.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
task3: task3.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<