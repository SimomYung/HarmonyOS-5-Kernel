# sched\_setscheduler()
Set a process's scheduling policy and its parameters

## Synopsis
```c
#include <sched.h>

int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);

struct sched_param {
    int sched_priority;
}
```

## Arguments

- **pid**
Process ID.
&nbsp;
- **policy**
A pointer to one of the scheduling policy identifier.

The SCHED_RESET_ON_FORK flag can be ORed in policy when calling sched_setscheduler(). As a result of including this flag, children created by fork/pthread_create do not inherit privileged scheduling policies.
&nbsp;
- **param**
A structure of scheduling policy parameters in the form of struct sched_param.

## Library
libc

## Description
Set a process's scheduling policy and the corresponding parameters. If *pid* is 0, the calling process is set. Currently, *policy* can be set to SCHED_FIFO, SCHED_RR, SCHED_NORMAL.

For RT policy: SCHED_FIFO, SCHED_RR, the permitted range of sched_priority is [1, 99]. The permitted sched_priority is always 0 for SCHED_NORMAL.

## Returns

- **0**
Success.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EINVAL**
1)*Invalid arguments*:*pid* is negative or *param* is NULL. It is the same with LINUX, but it is different from POSIX.

2)*policy* is not one of the recognized policies.

3)*param* dose not make sense for the specified policy.
&nbsp;
- **EPERM**
The caller process doesn't have enough privileges.
&nbsp;
- **ESRCH**
The process specified by pid can not be found.
&nbsp;
- **EFAULT**
Fail to copy data from user.
&nbsp;
- **EACCES**
Fail to check Mandatory Access Control.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
