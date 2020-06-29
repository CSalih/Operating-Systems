# Operating Systems Lab

This repository contains material required to complete exercises for the OS
lab, including exercise sheets and their associated materials.

## Schedule

| Exc                | Topic                         |
| ------------------ | ----------------------------- |
| [1](Assignment1)   | Shell scripting               |
| [2](Assignment2)   | Scheduling (P&P)              |
| [3](Assignment3)   | Processes, signals            |
| [4](Assignment4)   | FIFOs, pipes & message queues |
| [5](Assignment5)   | Shared memory & semaphores    |
| [6](Assignment6)   | Threads, mutexes, deadlocks   |
| [7](Assignment7)   | Spinlocks, cond vars, atomics |
| [8](Assignment8)   | Scheduling simulator          |
| [9](Assignment9)   | Membench, best-fit allocator  |
| [10](Assignment10) | Free list allocator           |

## General Information

All programming exercise solutions you hand in **must** compile properly on
the lab computers. The easiest way of ensuring this is by either developing
your solutions on a lab computer directly (outside of course hours), or by
connecting to
[ZID-GPL](https://www.uibk.ac.at/zid/systeme/linux/lpccs_4/benutzeranleitung_zid-gpl.html)
via SSH.

Note that the default version of the GCC compiler present on lab computers is
_extremely_ outdated. You may load a never version by calling `module load gcc/9.2.0`.

### Coding Guidelines

All programming exercises are conducted in C.

Your code should always compile without warnings, when passing the flags `-Wall -Wextra -Werror -std=gnu11`.

Make sure your code is properly
formatted using either your IDE/Text editor of choice, or by using a tool
such as `clang-format`.

Try to write _self-documenting code_ by choosing descriptive variable and
function names. While you may want to add comments to certain sections of
your code, try to avoid trivial comments such as `fopen(...); // open file`.
**All names and comments should be written in English**.

Finally, all submitted code must be accompanied by a GNU Makefile. That
is, your solution must be able to be compiled with a single call to `make`.

For some more specific topics such as proper error handling, check out our
[Frequently Asked Questions (FAQ)](FAQ.md).
