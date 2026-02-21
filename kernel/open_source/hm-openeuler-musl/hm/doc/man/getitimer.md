# getitimer()
Get the current value of an interval timer

## Synopsis
```c
#include <sys/time.h>;

int getitimer(int which, struct itimerval *value);
```

## Arguments
- **which**
Specify the timer on which to act.
&nbsp;
- **value**
Pointer to a *itimerval* structure to store the current value of the timer. The *itimerval* structure is defined as follows:

```c
struct timeval {
	time_t      tv_sec;         /* seconds */
	suseconds_t tv_usec;        /* microseconds */
};

struct itimerval {
	struct timeval it_interval; /* Interval for periodic timer */
	struct timeval it_value;    /* Time until next expiration */
};
```

## Library
libc

## Description
The getitimer() gets the current value of the timer specified by *which* and stores it in the *value* structure. The *it_value* field is set to the amount of time remaining until the next expiration of the timer, or zero if the timer is disabled. The *it_interval* field is set to the timer interval, zero returned indicates this is a one-shot timer.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
*value* is not a valid pointer.
&nbsp;
-  **EINVAL**
The value of *which* is not ITIMER_REAL.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
