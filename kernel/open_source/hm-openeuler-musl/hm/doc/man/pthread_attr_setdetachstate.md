# pthread\_attr\_setdetachstate()
Set the detach state attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **detachstate**
Detach state value, can be PTHREAD\_CREATE\_DETACHED or PTHREAD\_CREATE\_JOINABLE.

## Library
libc

## Description
The pthread\_attr\_setdetachstate() function sets the value specified by *detachstate* to the thread attributes object specified by *attr*. Valid values are PTHREAD\_CREATE\_DETACHED and PTHREAD\_CREATE\_JOINABLE.
&nbsp;
PTHREAD\_CREATE\_DETACHED means all threads created with *attr* are detached, calling pthread\_join() or pthread\_detach() on these threads will fail with EINVAL.
&nbsp;
PTHREAD\_CREATE\_JOINABLE means all threads created with *attr* are joinable, you should call pthread\_join() to wait child threads to exit and recycle their resources, otherwise there may exist memory leak. PTHREAD\_CREATE\_JOIN/ABLE is the default value used by pthread\_attr\_init().
&nbsp;
Calling pthread\_attr\_setdetachstate() if the *attr* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
Invalid detachstate value.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
