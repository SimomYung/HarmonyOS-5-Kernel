# pthread\_getschedparam()
Get a thread's scheduling policy and its parameters

## Synopsis
```c
#include <pthread.h>

int pthread_getschedparam(pthread_t thread, int *policy, const struct sched_param *param);

struct sched_param {
    int sched_priority;
};
```

## Arguments

- **thread**
Thread ID.
&nbsp;
- **policy**
A pointer to one of the scheduling policy identifier.
&nbsp;
- **param**
A structure of scheduling policy parameters in the form of struct sched_param.

## Library
libc

## Description
Get a thread's scheduling policy and the corresponding parameters identified by *thread*. Now the return value of *policy* can be set to SCHED_FIFO, SCHED_RR or SCHED_NORMAL.
&nbsp;
For RT policy: SCHED_FIFO, SCHED_RR, permitted range for scheduling priorities is [1, 99]. The permitted scheduling priorities is always 0 for SCHED_NORMAL.
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **ESRCH**
No thread with the thread ID could be found.
&nbsp;
- **EINVAL**
*param* is invalid.
&nbsp;
- **EPERM**
The caller thread doesn't have enough privileges.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
