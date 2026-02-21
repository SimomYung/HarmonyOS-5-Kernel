# adjtime()
Make small adjustments to the system clock

## Synopsis
```c
#include <sys/time.h>;

int adjtime(const struct timeval *delta, struct timeval *olddelta);
```

## Arguments
- **delta**
Pointer to a *timeval* structure where the amount of time by which the clock is to be adjusted.
&nbsp;
- **olddelta**
Useless argument.

## Library
libc

## Description
The adjtime() function gradually adjusts system clock. If *delta* is positive, the system clock is speeded
up by adding a small amount of time to the clock value in each second until the adjustment has been completed.
If *delta* is negative, the systme clock is slowed down in a similar way.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINVAL**
Failed to get data from vdso.
*delta* is an invalid pointer.

## Example
&nbsp;
## Classification
Unix

## Function Safety
TODO
