# timer_getoverrun()
Get overrun count for a POSIX per-process timer

## Synopsis
```c
#include <time.h>;

int timer_getoverrun(timer_t timerid);
```

## Arguments
- **timerid**
The ID of the timer.

## Library
libc

## Description
The timer_getoverrun() function gets the overrun count for the timer referred by *timerid*. An application can use the overrun count to accurately calculate the number of timer expirations that would have occurred over a given time interval. Notice, the timer_getoverrun() is accurate only when the interval of the timer is not less than the interval of the percpu tick timer.
&nbsp;
When a timer for which a signal is still pending expires, no signal shall be queued, and a timer overrun shall occur. The overrun count returned contains the number of extra timer expirations that occurred between the time the signal was generated (queued) and when it was delivered or accepted, up to but not including an implementation-defined maximum of *DELAYTIMER_MAX*. If the number of such extra expirations is greater than or equal to *DELAYTIMER_MAX*, then the overrun count shall be set to *DELAYTIMER_MAX*.

## Returns
On success, timer_getoverrun() returns the overrun count of the specified timer; this count may be 0 if no overruns have occurred. On failure, -1 is returned, and errno is set to indicate the error.

## Errors
-  **EINVAL**
*timerid* is not a valid timer ID.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
