# sigprocmask()
Examine and change blocked signals

## Synopsis
```c
#include <signal.h>

int sigprocmask(int how, const sigset_t *__restrict set, sigset_t *__restrict oldset);
```

## Arguments

- **how**
The handle method of the mask set.
&nbsp;
- **set**
The new signal mask set.
&nbsp;
- **oldset**
The old signal mask set.

## Library
libc

## Description
The sigprocmask() function is used to fetch and/or change the signal mask of the calling thread. If *oldset* is non-NULL, the previous value of the signal mask is stored in oldset. If *set* is NULL, then the signal mask is unchanged, but the current signal mask is returned in *oldset* (if it is not NULL). Currently, threads do not support to mask real-time signals, and signals which have not registered handler will not be blocked. The behavior of the call is dependent on the value of *how*, as follows:
&nbsp;
- **SIG_BLOCK**
The set of blocked signals is the union of the current set and the *set* argument.
&nbsp;
- **SIG_UNBLOCK**
The signals in *set* are removed from the current set of blocked signals.
&nbsp;
- **SIG_SETMASK**
The set of blocked signals is set to the argument *set*.
&nbsp;
An invalid *set* (such as -1) or *oldset* may cause segmentation fault.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
The *set* or *oldset* points to an address outside of the process's address space.
&nbsp;
- **EINVAL**
The value specified in *how* was invalid.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
