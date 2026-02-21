# pthread\_setaffinity\_np()
Set the CPU affinity of a given thread

## Synopsis
```c
#define _GNU_SOURCE
#include <pthread.h>

int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpu_set_t *cpuset);
```

## Arguments

- **thread**
The thread descriptor of the thread which to set the CPU affinity mask.
&nbsp;
- **cpusetsize**
The length(in bytes) of the buffer pointed to by *cpuset*.
&nbsp;
- **cpuset**
The pointer of the buffer to store the CPU affinity mask in.

## Library
libc

## Description
A thread's CPU affinity mask determines the set of CPUs on which the thread is able to run. The pthread\_setaffinity\_np() function can set the CPU affinity mask of the thread to the buffer pointed to by *cpuset*. If the call of pthread\_setaffinity\_np() is successful and the CPU on which the thread is running is not contained by *cpuset*, the thread will be migrated to a CPU contained by *cpuset*.
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **EFAULT**
*cpuset* points to a invalid address.
&nbsp;
- **EINVAL**
*cpusetsize* is smaller than the size of the affinity mask used by kernel.
&nbsp;
- **EINVAL**
*cpuset* contains no CPU that is currently online.
&nbsp;
- **ESRCH**
The thread specified by *thread* is not found.
&nbsp;
- **EPERM**
The caller process doesn't have enough privileges.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
