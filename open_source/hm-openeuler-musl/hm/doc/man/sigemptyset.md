# sigemptyset()
Initialize and empty a signal set

## Synopsis
```c
#include <signal.h>

int sigemptyset(sigset_t *set);
```

## Arguments

- **set**
The target signal set.

## Library
libc

## Description
The sigemptyset() function initializes the signal set pointed to by *set*, such that all signals are excluded.

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
