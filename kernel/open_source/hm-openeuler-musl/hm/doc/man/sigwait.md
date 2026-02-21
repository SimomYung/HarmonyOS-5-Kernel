# sigwait()
Wait for a signal

## Synopsis
```c
#include <signal.h>

int sigwait(const sigset_t *__restrict set, int *__restrict sig);
```

## Arguments

- **set**
The signal set which the function waits for.
&nbsp;
- **sig**
The signal that the function accepts and returns.

## Library
libc

## Description
The sigwait() function suspends the calling thread until one of the signals specified in the signal set *set* becomes pending. The function accepts the signal, removes it from the pending list of signals, and returns the signal number in *sig*.
&nbsp;
Only standard signals (1~34) are currently supported.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The *set* contains an invalid signal number.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
