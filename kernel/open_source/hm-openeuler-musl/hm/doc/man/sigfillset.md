# sigfillset()
Initialize and fill a signal set

## Synopsis
```c
#include <signal.h>

int sigfillset(sigset_t *set);
```

## Arguments

- **set**
The target signal set.

## Library
libc

## Description
The sigfillset() function initializes the signal set pointed to by *set*, such that all signals are included.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
