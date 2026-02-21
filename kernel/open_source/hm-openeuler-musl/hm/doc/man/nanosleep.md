# nanosleep()
High-resolution sleep

## Synopsis
```c
#include <time.h>

int nanosleep(const struct timespec *req, struct timespec *rem);
```

## Arguments
- **req**
Pointer to a *timespec* structure where the expected suspend time stores. The *timespec* structure is defined as follows:

```c
struct timespec{
	time_t tv_sec;   /* seconds */
	long   tv_nsec;  /* nanoseconds */
};
```
&nbsp;
- **rem**
Pointer to a *timespec* structure stores the amount of time remaining when the nanosleep() function returns because it has been interrupted by a signal, NULL is allowed.

## Library
libc

## Description
The nanosleep() function makes the calling thread to be suspended from execution until either the time interval specified by the *req* argument has elapsed or a signal arrives. When the function is interrupted by a signal, the remaining time is stored in *rem* if it is non-NULL.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred or interrupted by a signal.

## Errors
- **EFAULT**
*req* or *rem* is an invalid pointer.
&nbsp;
- **EINTR**
Interrupted by a signal.
&nbsp;
- **EINVAL**
The value in the *tv_nsec* field is not in the range 0 to 999999999 or *tv_sec* is negative.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
