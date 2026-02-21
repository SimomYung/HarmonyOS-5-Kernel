# sched\_getscheduler()
Get a process's scheduling policy

## Synopsis
```c
#include <sched.h>

int sched_getscheduler(pid_t pid);
```

## Arguments

- **pid**
Process ID.


## Library
libc

## Description
Get a process's scheduling policy. If *pid* is 0, get the calling process's scheduling policy. Currently, the return value can be set to SCHED_FIFO, SCHED_RR or SCHED_NORMAL.

## Returns

- **nonnegative**
Get process *pid*'s scheduling policy.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EINVAL**
*pid* is negative. It is the same with LINUX, but it is different from POSIX.
&nbsp;
- **EPERM**
The caller process doesn't have enough privileges.
&nbsp;
- **ESRCH**
The process specified by pid can not be found.
&nbsp;
- **EACCES**
Fail to check Mandatory Access Control.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
