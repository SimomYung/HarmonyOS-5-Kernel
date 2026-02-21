# sigaddset()
Add a signal to a signal set

## Synopsis
```c
#include <signal.h>

int sigaddset(sigset_t *set, int signo);
```

## Arguments

- **set**
The target signal set.
&nbsp;
- **signo**
The signal to be added in the signal set.

## Library
libc

## Description
The sigaddset() function adds the individual signal specified by the *signo* to the signal set which the *set* points to.
&nbsp;
The signals 32~34 are not currently supported. Try to add 32~34 to *set* is not feasible, it will return -1.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*signo* is invalid.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
