# setitimer()
Set the value of an interval timer

## Synopsis
```c
#include <sys/time.h>;

int setitimer(int which, const struct itimerval *restrict new_value,
			  struct itimerval *restrict old_value);
```

## Arguments
- **which**
Specify the timer on which to act, only ITIMER_REAL is supported.
&nbsp;
- **new_value**
Pointer to a *itimerval* structure to store the time value set to the timer. The *itimerval* structure is defined as follows:

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
&nbsp;
- **old_value**
Pointer to a *itimerval* structure to store the previous value of the timer, NULL is allowed.

## Library
libc

## Description
The setitimer() sets the timer to the value in *new_value*. If *old_value* is non-NULL, the old value of the timer (i.e., the same information as returned by getitimer()) is stored there.
&nbsp;
The timer decrements in real time from *it_value* to zero, sends SIGALRM to the caller thread, and resets to *it_interval*. When being set to zero, the timer stops. When the caller thread exited before the timer expires, the timer will be canceled and the process cannot receive the signal any more.
&nbsp;
If the interval between two timers is very short (depends on the system timer resolution) or the system load is heavy, the timer may expire before the signal from a previous expiration has been delivered. The second signal in such an event will be lost.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
*new_value* or *old_value* is not a valid pointer.
&nbsp;
-  **EINVAL**
The value of *which* is not ITIMER_REAL.
*new_value.it_value.tv_sec* is negative or *new_value.it_value.tv_usec* is negative or greater than 999,999.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
