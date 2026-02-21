# gettimeofday()
Get the current time

## Synopsis
```c
#include <sys/time.h>;

int gettimeofday(struct timeval *restrict tp, void *restrict tzp);
```

## Arguments
- **tp**
Pointer to a *timeval* structure where the current time is stored. The *timeval* structure is defined as follows:

```c
struct timeval {
	time_t      tv_sec;  /* seconds */
	suseconds_t tv_usec; /* microseconds */
};
```
&nbsp;
- **tzp**
Useless argument.

## Library
libc

## Description
The gettimeofday() function gets the current time, expressed as seconds and microseconds since the Epoch, and stores it in *tp*.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
N/A
## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO