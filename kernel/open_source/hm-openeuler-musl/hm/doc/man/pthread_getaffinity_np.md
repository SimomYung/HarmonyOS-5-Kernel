# pthread\_getaffinity\_np()
Get the cpu affinity mask of a thread

## Synopsis
```c
#define _GNU_SOURCE
#include <pthread.h>

int pthread_getaffinity_np(pthread_t thread, size_t cpusetsize,
                           cpu_set_t *cpuset);
```

## Arguments

- **thread**
The target thread descriptor.
&nbsp;
- **cpusetsize**
The length (in bytes) of the buffer pointed to by *cpuset*.
&nbsp;
- **cpuset**
A pointer to the buffer that stores the returned cpu affinity mask.

## Library
libc

## Description
The pthread\_getaffinity\_np() function obtains the cpu affinity mask of the target thread specified by *thread* and places the returned value to the buffer indicated by *cpuset*, the length (in bytes) of this buffer is *cpusetsize* which is typically assigned to sizeof(cpu_set_t).
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
*cpusetsize* is smaller than the size of the affinity mask used by the kernel.
&nbsp;
- **EFAULT**
*cpuset* is NULL or it is an illegal address.
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
