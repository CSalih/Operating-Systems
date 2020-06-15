CC = gcc
LD = gcc

CFLAGS = -std=gnu99 -Wall -Werror
LDFLAGS= -pthread

.PHONY: all task1 task2 clean

all: task1 task2

clean:
	$(RM) task1 task2

task1: task1.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
task2: task2.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<