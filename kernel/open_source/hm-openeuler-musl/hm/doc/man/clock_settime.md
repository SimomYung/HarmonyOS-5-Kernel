# clock_settime()
Set the specified clock

## Synopsis
```c
#include <times.h>;

int clock_settime(clockid_t clock_id, const struct timespec *tp);
```

## Arguments
- **clock_id**
The ID of the particular clock on which to act.
&nbsp;
- **tp**
Pointer to a *timespec* where the value to be set to the clock is stored.

## Library
libc

## Description
The clock_settime() function sets the value specified by *tp* to the specified clock. The *clock_id* only supports CLOCK_REALTIME.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINVAL**
The *clock_id* is not supported.
The *tp* argument specified a nanosecond value less than zero or greater than or equal to 1000000000.
&nbsp;
- **EFAULT**
*tp* points outside the accessible address space.
## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
