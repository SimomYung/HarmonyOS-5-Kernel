# sigwaitinfo()
Wait for a signal

## Synopsis
```c
#include <signal.h>

int sigwaitinfo(const sigset_t *__restrict set, siginfo_t *__restrict info);
```

## Arguments

- **set**
The signal set which the function waits for.
&nbsp;
- **info**
The info of the signal which the calling thread accepts and returns.

## Library
libc

## Description
The sigwaitinfo() function suspends the calling thread until one of the signals specified in the signal set *set* becomes pending. The function accepts the signal, removes it from the pending list of signals, and returns the information of the signal to the buffer which the *info* points to. If one of the signals in *set* is already pending, sigwaitinfo() will return immediately.
&nbsp;
Only standard signals (1~34) are currently supported. If the *info* is invalid, the coredump would occur.

## Returns

- **signo**
A signo greater than zero returns on success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINTR**
The wait was interrupted by a signal.
&nbsp;
- **EINVAL**
The parameter *set* was invalid.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
