# pthread\_getattr\_np()
Get the thread attributes

## Synopsis
```c
#define _GNU_SOURCE
#include <pthread.h>

int pthread_getattr_np(pthread_t thread, pthread_attr_t *attr);
```

## Arguments

- **thread**
The target thread descriptor.
&nbsp;
- **attr**
A pointer to the buffer which stores the returned thread attributes value.

## Library
libc

## Description
The pthread\_getattr\_np() function fills the buffer specified by *attr* with the actual attribute values describing the target thread specified by *thread*. Afterwards, the *attr* can be used to a series of pthread\_attr\_get prefixed functions to get the particular attribute value.
&nbsp;
The returned attribute values may differ from the corresponding attribute values passed in the *attr* that is used to create *thread* using pthread\_create(). In particular, the following attribute may differ:
&nbsp;
* The detach state attribute. Since a joinable thread can detach itself at runtime.
&nbsp;
* The stack size and guard size attribute. Since they will be rounded up to a multiple of the PAGE\_SIZE.
&nbsp;
* The stack address attribute. Since the implementation of pthread needs some space (in stack) to maintain the internal data, the actual stack that can be used by the thread may be less than the size of the buffer that is passed in pthread\_attr\_setstack().
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **ESRCH**
The thread specified by *thread* is not found.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
