# pthread\_mutexattr\_settype()
Set the mutex type attribute

## Synopsis
```c
#include <pthread.h>

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
```

## Arguments

- **attr**
A pointer to the mutex attributes object.
&nbsp;
- **type**
Mutex type to be set.

## Library
libc

## Description
The pthread\_mutexattr\_settype() function sets the mutex type attribute specified by *type* in *attr*. Valid *type* values are: PTHREAD\_MUTEX\_NORMAL, PTHREAD\_MUTEX\_ERRORCHECK, PTHREAD\_MUTEX\_RECURSIVE and PTHREAD\_MUTEX\_DEFAULT. Default value set in *attr* after pthread\_mutexattr\_init() is PTHREAD\_MUTEX\_DEFAULT (mapped to PTHREAD\_MUTEX\_NORMAL).
&nbsp;
The type of a mutex affects the behavior of pthread\_mutex\_lock() and pthread\_mutex\_unlock() in the following manner:
1. Trying to relock a mutex that one has already locked. (described in the Relock column of the following table)
2. Trying to unlock a mutex that one has not locked or a mutex which is in the unlocked state. (described in the Unlock When Not Owner column of the following table)
&nbsp;

| Mutex Type | Robustness | Relock | Unlock When Not Owner |
|------------|------------|--------|-----------------------|
| NORMAL/DEFAULT | N | deadlock | undefined behavior |
| NORMAL/DEFAULT | Y | deadlock | return error |
| ERRORCHECK | Y/N | return error | return error |
| RECURSIVE | Y/N | recursive lock | return error |

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
Invalid *type* value.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
