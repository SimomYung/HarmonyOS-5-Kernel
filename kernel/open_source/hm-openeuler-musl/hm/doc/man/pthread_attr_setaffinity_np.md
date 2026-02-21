# pthread\_attr\_setaffinity\_np()
Set the cpu affinity mask attribute in the thread attributes object

## Synopsis
```c
#define _GNU_SOURCE
#include <pthread.h>

int pthread_attr_setaffinity_np(pthread_attr_t *attr,
                                size_t cpusetsize, const cpu_set_t *cpuset);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **cpusetsize**
The length (in bytes) of the buffer pointed to by *cpuset*.
&nbsp;
- **cpuset**
A pointer to the buffer that stores the value of the cpu affinity mask to be set.

## Library
libc

## Description
The pthread\_attr\_setaffinity\_np() function sets the cpu affinity mask attribute specified by *cpuset* to the thread attributes object specified by *attr*. The length (in bytes) of the buffer pointed to by *cpuset* is *cpusetsize* which is typically assigned to sizeof(cpu\_set\_t).
&nbsp;
Note that it is not checked whether *cpuset* specifies a cpu that is outside the set supported by the kernel or *cpuset* specifies a cpu that lies outside the range indicated by *cpusetsize*. If you pass an *attr* containing invalid cpu affinity mask to pthread\_create() to create a new thread, pthread\_create() will fail with EINVAL.

## Returns

- **0**
Success.
&nbsp;
- **ENOMEM**
The system lacks enough memory to set the cpu affinity mask to *attr*.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
