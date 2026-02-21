# sigismember()
Test if a signal is in a signal set

## Synopsis
```c
#include <signal.h>

int sigismember(const sigset_t *set, int signo);
```

## Arguments

- **set**
The target signal set.
&nbsp;
- **signo**
The signal to be tested.

## Library
libc

## Description
The sigismember() function tests whether the signal *signo* is a member of the set which the *set* points to.

## Returns

- **1**
The specified signal is a member of the specified set.
&nbsp;
- **0**
The specified signal is not a member of the specified set or signo is greater than SIGRTMAX.
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
