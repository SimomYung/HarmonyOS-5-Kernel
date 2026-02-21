# sched\_setparam()
Set a process's parameters of scheduling

## Synopsis
```c
#include <sched.h>

int sched_setparam(pid_t pid, const struct sched_param *param);

struct sched_param {
    int sched_priority;
}
```

## Arguments

- **pid**
Process ID.
&nbsp;
- **param**
A structure of scheduling policy parameters in the form of struct sched_param.


## Library
libc

## Description
Set a process's parameters of scheduling. If *pid* is 0, set the calling process's parameters of scheduling. For RT policy: SCHED_FIFO, SCHED_RR, the permitted range for sched_priority is [1, 99].

The sched_priority of SCHED_NORMAL is always 0.

## Returns

- **0**
Success.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EINVAL**
1)*Invalid arguments*:*pid* is negative or param is NULL. It is the same with LINUX, but it is different from POSIX.

2)The armument param does not make sense for the current scheduling policy.
&nbsp;
- **EPERM**
The caller process doesn't have enough privileges.
&nbsp;
- **ESRCH**
The process specified by *pid* can not be found.
&nbsp;
- **EFAULT**
Fail to copu data from user.
&nbsp;
- **EACCES**
Fail to check Mandatory Access Control.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
