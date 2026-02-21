# timer_gettime()
Fetch state of a POSIX per-process timer

## Synopsis
```c
#include <time.h>

int timer_gettime(timer_t timerid, struct itimerspec *curr_value);
```

## Arguments
- **timerid**
The ID of the timer.
&nbsp;
- **curr_value**
Points to an itimerspec structure that is used to return the time until next expiration, and the interval, for the timer specified by *timerid*. The *itimerspec* structure is defined as follows:

```c
struct timespec {
	time_t tv_sec;               /* Seconds */
	long   tv_nsec;              /* Nanoseconds */
};

struct itimerspec {
	struct timespec it_interval; /* Timer interval */
	struct timespec it_value;    /* Initial expiration */
};
```
## Library
libc

## Description
The timer_gettime() function fetches the time until next expiration, and the interval, for the timer specified by *timerid*,in the buffer pointed to by *curr_value*. The time remaining until the next timer expiration is returned in *curr_value->it_value*; this is always a relative value. If the value returned in *curr_value->it_value* is zero, then the timer is currently disarmed. The timer interval is returned in *curr_value->it_interval*. If the value returned in *curr_value->it_interval* is zero, then this is a "one-shot" timer.
## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
*curr_value* is not a valid pointer.
&nbsp;
-  **EINVAL**
*timerid* is invalid.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
