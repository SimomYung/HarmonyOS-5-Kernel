# pthread\_detach()
Detach a thread

## Synopsis
```c
#include <pthread.h>

int pthread_detach(pthread_t thread);
```

## Arguments

- **thread**
The target thread descriptor.

## Library
libc

## Description
The pthread\_detach() function detaches the target thread specified by *thread*. Resources consumed by a detached thread will be released automatically when it terminates without the need for another thread to join with the terminated thread.
&nbsp;
Note that you should pass a valid thread descriptor to this function. If the thread has finished its life cycle, segmentation fault will occur.
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The target thread has already been detached.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
