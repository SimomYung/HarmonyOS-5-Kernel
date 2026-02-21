# timer_settime()
Arm or disarm a POSIX per-process timer

## Synopsis
```c
#include <time.h>;

int timer_settime(timer_t timerid, int flags,
				  const struct itimerspec *restrict new_value,
				  struct itimerspec *restrict old_value);
```

## Arguments
- **timerid**
The ID of the timer.
&nbsp;
- **flags**
The flag indicates the *new_value* is a relative value or an absolute value.
&nbsp;
- **new_value**
Points to an *itimerspec* structure that specifies the new initial value and the new interval for the timer. The *itimerspec* structure is defined an follows:

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
&nbsp;
- **old_value**
Points to an itimerspec structure that is used to return the previous interval of the timer and the amount of time until the timer would previously have next expired, NULL is allowed.

## Library
libc

## Description
The timer_settime() function arms or disarms the timer identified by *timerid*. If *new_value->it_value* is a nonzero value, then timer_settime() arms the timer, setting it to initially expire at the given time. If the timer was already armed, then the previous settings are overwritten. If *new_value->it_value* is a zero value, then the timer is disarmed. Notice, at most one timer is armed in a thread. For a timer which sevp.sigev_notify field is SIGEV_SIGNAL, when the timer expires, the sigev_signo signal is generated for the thread (not the process) that called timer_settime.
&nbsp;
The *new_value->it_interval* field specifies the period of the timer. If this field is nonzero, then each time that an armed timer expires, the timer is reloaded from the value specified in *new_value->it_interval*. If *new_value->it_interval* specifies a zero value, then the timer expires just once at the time specified by *it_value*.
&nbsp;
The initial expiration time specified in *new_value->it_value* is relative to the current time on the timer's clock at the time of the call. The absolute time (i.e., TIMER_ABSTIME flag) is supported now.
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
*timerid* is invalid.
*new_value.it_value* is negative or *new_value.it_value.tv_nsec* is negative or greater than 999,999,999.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
