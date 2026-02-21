# sigtimedwait()
Wait for a signal

## Synopsis
```c
#include <signal.h>

int sigtimedwait(const sigset_t *__restrict set,
		 siginfo_t *__restrict info,
		 const struct timespec *__restrict timeout);
```

## Arguments

- **set**
The signal set which the function waits for.
&nbsp;
- **info**
The info of the signal which the calling thread accepts and returns.
&nbsp;
- **timeout**
The interval for which the thread is suspended waiting for a signal.

## Library
libc

## Description
The sigtimedwait() function operates in exactly the same way as sigwaitinfo(), except that it has an additional *timeout*.
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
- **EAGAIN**
No signal in *set* was became pending within the *timeout* period.
&nbsp;
- **EINVAL**
The *timeout* was invalid.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
