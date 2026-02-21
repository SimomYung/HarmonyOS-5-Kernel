# clock_nanosleep()
High-resolution sleep with specifiable clock

## Synopsis
```c
#include <time.h>

int clock_nanosleep(clockid_t clock_id, int flags,
					const struct timespec *request,
					struct timespec *remain);
```

## Arguments
- **clock_id**
The clock used to measure the time.
&nbsp;
- **flags**
The flag indicates the *request* is a relative time or an absolute time.
&nbsp;
- **request**
Pointer to a *timespec* structure where the expected suspend time stores.
&nbsp;
- **remain**
Pointer to a *timespec* structure stores the amount of time remaining when the clock_nanosleep() function returns because it has been interrupted by a signal, NULL is allowed.

## Library
libc

## Description
The clock_nanosleep() makes the calling thread to be suspended from execution like nanosleep(). It differs in allowing the caller to select the clock against which the sleep interval is to be measured, and in allowing the sleep interval to be specified as either an absolute or a relative value.
&nbsp;
The *clock_id* can be CLOCK_REALTIME or CLOCK_MONOTONIC.
&nbsp;
If *flags* is 0, then the value specified in *request* is interpreted as an interval relative to the current value of the clock specified by *clock_id*. If *flags* is TIMER_ABSTIME, the *request* is interpreted as an absolute time as measured by the *clock_id* clock. If *request* is less than or equal to the current value of the clock, then clock_nanosleep() returns immediately. Attention, clock_nanosleep is implemented by using futex, but the absolute time clock is not supported for the current futex. If *flags* is TIMER_ABSTIME, we will convert it to 0.
&nbsp;
For CLOCK_REALTIME clock, after changing the time via clock_settime, doesn't support using the new clock time to determine the time at which an absolute clock_nanosleep() will wake up.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred or interrupted by a signal.

## Errors
- **EFAULT**
*request* or *remain* is an invalid pointer.
&nbsp;
- **EINTR**
Interrupted by a signal.
&nbsp;
- **EINVAL**
The value in the *tv_nsec* field is not in the range 0 to 999999999 or *tv_sec* is negative, or the *clock_id* is invalid.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
