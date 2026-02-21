# raise()
Send signal to the caller

## Synopsis
```c
#include <signal.h>

int raise(int sig);
```

## Arguments

- **sig**
The signum to be sent.

## Library
libc

## Description
The raise() function sends a signal to the calling process or thread.
&nbsp;
In a single-thread process, it is equivalent to kill(getpid(), sig). In a multi-thread process, it is equivalent to pthread_kill(pthread_self(), sig). If the signal causes a handler to be called, raise() will return after the signal handler has returned.

## Returns

- **0**
Signal was sent successfully.
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
