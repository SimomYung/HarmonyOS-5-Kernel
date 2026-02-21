# sched\_yield()
Relinquish the cpu

## Synopsis
```c
#include <sched.h>

int sched_yield(void);
```

## Arguments
N/A
&nbsp;

## Library
libc

## Description
The calling process relinquishs the cpu. It is put at the end of the scheduling queue identified by its static priority, and the process at the head of the scheduling queue runs.

## Returns

- **0**
Always success.
&nbsp;

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
