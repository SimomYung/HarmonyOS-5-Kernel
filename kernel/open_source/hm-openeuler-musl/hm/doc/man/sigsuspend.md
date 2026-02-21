# sigsuspend()
Wait for a signal

## Synopsis
```c
#include <signal.h>

int sigsuspend(const sigset_t *mask);
```

## Arguments

- **mask**
The signal mask which is temporarily set to the calling thread.

## Library
libc

## Description
The sigsuspend() temporarily replaces the signal mask of the calling thread with the *mask*, and then suspends the calling thread, until delivery of a signal which will invoke a handler or terminate the process.
&nbsp;
The sigsuspend() does not return if a signal terminates the process. If a signal is caught, the sigsuspend() returns after the handler returns, and the mask is restored to the state before the thread calls the sigsuspend().
&nbsp;
Only standard signals (1~34) are currently supported. Try to block SIGKILL or SIGSTOP in *mask* is not feasible.
&nbsp;
Note that you should pass a valid *mask* to this function, invalid *mask* (such as -1) may cause segmentation fault.

## Returns

Always returns -1.

## Errors

- **EINTR**
The call was interrupted by a signal.
&nbsp;
- **EFAULT**
The pointer of *mask* was invalid.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
