# nice()
Change process priority

## Synopsis
```c
#include <unistd.h>

int nice(int inc);
```

## Arguments

- **inc**
The nice value you want to increase.

## Library
libc

## Description
nice() adds *inc* to the nice value for the calling thread.
&nbsp;
Our system only supports setting the nice value of the ICES scheduling algorithm.

## Returns

On success, the new nice value is returned. On error, -1 is returned, and errno is set appropriately.

A successful call can legitimately return -1. To detect an error, set errno to 0 before the call, and check whether it is nonzero after nice() returns -1.

## Errors

- **EPERM**
The calling process attempted to increase its priority by supplying a negative inc but has insufficient privileges.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
