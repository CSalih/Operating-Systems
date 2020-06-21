# UIBK Operating Systems Lab 2020

This repository contains material required to complete exercises for the OS
lab in the 2020 summer semester, including exercise sheets and their
associated materials. Note that some of this material is based on previous
years.

**Make sure to read this document carefully, as it contains vital information
regarding the grading of exercises.**

## Schedule

This schedule gives you an overview of the topics we will discuss on each
date. It will be updated continuously throughout the semester. Note that
exercises associated with a given date are to be discussed on that day, i.e.,
solutions for that exercise have to be handed in via OLAT by 17.00 on the
previous day (Tuesdays).

| Exc                | Topic                         | Notes   |
| ------------------ | ----------------------------- | ------- |
| [1](Assignment1)   | Shell scripting               |         |
| [2](Assignment2)   | Scheduling (P&P)              |         |
| [3](Assignment3)   | Processes, signals            |         |
| [4](Assignment4)   | FIFOs, pipes & message queues |         |
| [5](Assignment5)   | Shared memory & semaphores    |         |
| [6](Assignment6)   | Threads, mutexes, deadlocks   |         |
| [7](Assignment7)   | Spinlocks, cond vars, atomics |         |
| [8](Assignment8)   | Scheduling simulator          |         |
| [9](Assignment9)   | Membench, best-fit allocator  |         |
| [10](Assignment10) | Free list allocator           |         |

## General Information

All programming exercise solutions you hand in **must** compile properly on
the lab computers. The easiest way of ensuring this is by either developing
your solutions on a lab computer directly (outside of course hours), or by
connecting to
[ZID-GPL](https://www.uibk.ac.at/zid/systeme/linux/lpccs_4/benutzeranleitung_zid-gpl.html)
via SSH.

Note that the default version of the GCC compiler present on lab computers is
_extremely_ outdated. You may load a never version by calling `module load gcc/9.2.0`.

### Connecting to ZID-GPL via SSH

**NOTE**: You have to be inside the University network to be able to connect
via SSH. See [here](https://www.uibk.ac.at/zid/netz-komm/vpn/) for more
information.

To log into ZID-GPL via SSH, use the following command on Linux/Mac OSX:

`ssh -l <username> zid-gpl.uibk.ac.at`

On Windows, you can use Powershell (Windows 10) or a third-party SSH client
such as [PuTTY](https://www.putty.org/).

### Coding Guidelines

All programming exercises are conducted in C.

Your code should always compile without warnings, when passing the flags `-Wall -Wextra -Werror -std=gnu11`.

Make sure your code is properly
formatted using either your IDE/Text editor of choice, or by using a tool
such as `clang-format`. **Failure to consistently format code may result in lower scores.**

Try to write _self-documenting code_ by choosing descriptive variable and
function names. While you may want to add comments to certain sections of
your code, try to avoid trivial comments such as `fopen(...); // open file`.
**All names and comments should be written in English**.

Finally, all submitted code must be accompanied by a GNU Makefile. That
is, your solution must be able to be compiled with a single call to `make`.

For some more specific topics such as proper error handling, check out our
[Frequently Asked Questions (FAQ)](FAQ.md).

## Grading

Grades are based on the _number of exercises_ you complete (1 point per
exercise), the _quality of your presentation_ of these exercises, and **a
written test**. This test will be a programming assignment where you need to
implement a small C program similar in scope to the exercises.

Please **do not underestimate this test**, and make sure to use the
self-assessment exercise to understand whether you are ready to successfully
complete this lab.

**Attendence** is mandatory, failing to attend more than two times will likely
result in a failing grade. Exercises only count as completed if the lab in
which they are presented is attended.

**Note:**
In order to successfully complete this course, you need to get a positive
grade on _the test_ **and** _the number of exercises solved and their
presentation_. Both tests and the exercises/presentation contribute to your
final grade.
