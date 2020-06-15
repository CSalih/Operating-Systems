CC = gcc
LD = gcc

CFLAGS = -std=gnu11 -Wall -Werror
LDFLAGS= -pthread
CPPFLAGS= -COMPLILE_TASK3a

.PHONY: all task1 task2 task3a task3b clean

all: task1 task2 task3a task3b

clean:
	$(RM) task1 task2 task3a task3b

task1: task1.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
task2: task2.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
task3a: task3.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)  -o $@ $<
task3b: task3.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<