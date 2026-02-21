# clock_getcpuclockid()
Get the clock ID of a process CPU-time clock

## Synopsis
```c
#include <time.h>;

int clock_getcpuclockid(pid_t pid, clockid_t *clk);
```

## Arguments
- **pid**
The process ID.
&nbsp;
- **clk**
Pointer to a *clockid_t* where the clock ID of the CPU-time clock of the process *pid* is stored.

## Library
libc

## Description
The clock_getcpuclockid() function gets the clock ID of the CPU-time clock of the process *pid*, and stores it in *clk*.

## Returns
- **0**
Success.
&nbsp;
- **other positive number**
An error occurred.

## Errors
- **EINVAL**
The *pid* is not a calling process.
*clk* is NULL.
&nbsp;
- **EFAULT**
The kernel has not enough space.
*clk* is NULL.
## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
