# pthread_getcpuclockid()
Get the clock ID of the CPU-time clock from a specified thread.

## Synopsis
```c
#include <pthread.h>;
#include <time.h>;

int pthread_getcpuclockid(pthread_t t, clockid_t *clk);
```

## Arguments
- **t**
The thread ID.
&nbsp;
- **clk**
Pointer to a *clockid_t* where the clock ID is stored.

## Library
libc

## Description
The pthread_getcpuclockid() function gets the clock ID for the CPU-time clock of the thread *t*, and stores it in *clk*.

## Returns
- **0**
Success.
&nbsp;
- **nonzero number**
An error occurred.

## Errors
- **ESRCH**
No thread with thread id *t*.
*clk* is NULL.
## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
