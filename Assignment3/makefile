CC = gcc
LD = gcc

CFLAGS = -Wall -Wextra -Werror -std=gnu11

.PHONY: all task1 task2 task3 clean

all: task1 task2 task3

clean:
	$(RM) task1 task2 task3

task1: task1.c
	$(CC) $(CFLAGS) -o $@ $<
task2: task2.c
	$(CC) $(CFLAGS) -o $@ $<
task3: task3.c
	$(CC) $(CFLAGS) -o $@ $<