# pthread\_setschedprio()
Set a thread's scheduling priority

## Synopsis
```c
#include <pthread.h>

int pthread_setschedprio(pthread_t thread, int prio);
```

## Arguments

- **thread**
Thread ID.
&nbsp;
- **prio**
The scheduling priority.


## Library
libc

## Description
Set the thread *thread*'s scheduling priority specified in *prio*.
&nbsp;
For RT policy: SCHED_FIFO, SCHED_RR, the permitted range for scheduling priorities is [1, 99].
&nbsp;
The scheduling priority of SCHED_NORMAL is always 0.
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
*prio* is not valid for the scheduling policy of the specified thread.
&nbsp;
- **ESRCH**
No thread with the ID thread could be found.
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
