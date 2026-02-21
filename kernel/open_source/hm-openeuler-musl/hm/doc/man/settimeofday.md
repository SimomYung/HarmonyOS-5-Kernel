# settimeofday()
Set the date and time

## Synopsis
```c
#include <sys/time.h>;

int settimeofday(const struct timeval *tv, const struct timezone *tz);
```

## Arguments
- **tv**
Pointer to a *timeval* structure where the value to be set is stored.
&nbsp;
- **tz**
Useless argument.

## Library
libc

## Description
The settimeofday() function sets the current time, like clock_settime() setting the CLOCK_REALTIME clock value.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
*tv* is an invalid pointer.

## Example
&nbsp;
## Classification
Unix

## Function Safety
TODO
