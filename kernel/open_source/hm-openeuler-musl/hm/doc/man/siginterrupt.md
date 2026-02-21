# siginterrupt()
Allow signals to interrupt system calls

## Synopsis
```c
#include <signal.h>

int siginterrupt(int sig, int flag);
```

## Arguments

- **sig**
The signal to be set.
&nbsp;
- **flag**
The flag indicates whether the restart function enables.

## Library
libc

## Description
The siginterrupt() function changes the restart behavior when a system call is interrupted by the signal *sig*. If the *flag* is false (0), then the system call will be restarted. If the *flag* is true (1), the system call interrupted by the signal *sig* will return -1 and *errno* will be set to EINTR.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*sig* is invalid.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
