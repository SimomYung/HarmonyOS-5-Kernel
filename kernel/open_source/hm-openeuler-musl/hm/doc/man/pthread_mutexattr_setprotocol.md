# pthread\_mutexattr\_setprotocol()
Set the protocol attribute of the mutex attributes object

## Synopsis
```c
#include <pthread.h>

int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr,
                                  int protocol);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **protocol**
The protocol attribute to be set.

## Library
libc

## Description
The pthread\_mutexattr\_setprotocol() function sets the protocol attribute specified by *protocol* to the mutex attributes object specified by *attr*. This attribute affects the priority and scheduling of the thread which owns the mutex initialized with *attr*.
&nbsp;
The default value in *attr* after initialization through pthread\_mutexattr\_init() is PTHREAD\_PRIO\_NONE. When a thread owns a mutex with the PTHREAD\_PRIO\_NONE protocol attribute, its priority and scheduling will not be affected by its mutex ownership.
&nbsp;
Another supported value of *protocol* is PTHREAD\_PRIO\_INHERIT when CONFIG\_RT\_FUTEX is enabled. If a thread calls pthread\_mutex\_lock() on a mutex which is initialized with the protocol attribute having the value PTHREAD\_PRIO\_INHERIT, when the calling thread is blocked because the mutex is owned by another thread, then the priority of the owner thread will be promoted (if the priority of the owner thread has lower priority) to the same priority of the calling thread as long as it continues to own the mutex. Furthermore, if this owner thread itself is blocked on another mutex with the PTHREAD\_PRIO\_INHERIT protocol, the same priority inheritance effect will be propagated to this other owner thread, in a recursive manner.

## Returns

- **0**
Success.
&nbsp;
- **ENOTSUP**
The value specified by *protocol* equals to PTHREAD\_PRIO\_PROTECT.
&nbsp;
- **EINVAL**
The value specified by *protocol* is invalid.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
