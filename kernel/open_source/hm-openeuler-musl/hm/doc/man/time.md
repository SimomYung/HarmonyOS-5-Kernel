# time()
Get time in seconds

## Synopsis
```c
#include <time.h>;

time_t time(time_t *tloc);
```

## Arguments
- **tloc**
Points to an area where the return value is stored. If *tloc* is a NULL pointer, no value is stored.

## Library
libc

## Description
The time() function returns the value of time in seconds since the Epoch.

## Returns
The value returned is the time in seconds.
&nbsp;
- **(time\_t ) -1**
An error occurred.

## Errors
- **EFAULT**
*tloc* points outside the accessible address space.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO