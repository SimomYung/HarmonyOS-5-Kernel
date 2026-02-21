# pthread\_condattr\_setclock()
Set the clock selection condition variable attribute

## Synopsis
```c
#include <pthread.h>

int pthread_condattr_setclock(pthread_condattr_t *attr,
                              clockid_t clock_id);
```

## Arguments

- **attr**
A pointer to the condition variable attributes object.
&nbsp;
- **clock_id**
The clock ID, can be CLOCK\_MONOTONIC or CLOCK\_REALTIME.

## Library
libc

## Description
The pthread\_condattr\_setclock() function sets the clock ID specified by *clock_id* in the condition variable attributes object specified by *attr*. Valid values are CLOCK\_MONOTONIC and CLOCK\_REALTIME (the default value). The clock is used to measure the timeout service of pthread\_cond\_timedwait().

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
Invalid *clock_id* value (not CLOCK_MONOTONIC or CLOCK_REALTIME).

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
