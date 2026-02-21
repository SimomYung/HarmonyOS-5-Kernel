# timer_create()
Create a POSIX per-process timer

## Synopsis
```c
#include <signal.h>
#include <time.h>;

int timer_create(clockid_t clockid, struct sigevent *restrict evp,
				 timer_t *restrict timerid);
```

## Arguments
- **clockid**
The clock that the new timer uses to measure time.
&nbsp;
- **evp**
Points to a sigevent structure that specifies how the caller should be notified when the timer expires.
&nbsp;
- **timerid**
Points to the buffer where the ID of the new timer is returned.

## Library
libc

## Description
The timer_create() function creates a new per-process interval timer. The ID of the new timer is returned in *timerid*, which must be a non-NULL pointer. This ID is unique within the process, until the timer is deleted. The new timer is initially disarmed.
&nbsp;
For *clockid*, only CLOCK_MONOTONIC, CLOCK_REALTIME and CLOCK_BOOTTIME are supported.
&nbsp;
The *sevp.sigev_notify* field can have the following values:

- **SIGEV_SIGNAL**
Upon timer expiration, generate the signal *sigev_signo* for the timer_settime() caller thread. The *si_code* field of *siginfo_t* structure will be set to SI_TIMER. Passing the *sigev_value* is not supported.
&nbsp;
- **SIGEV_THREAD**
Upon timer expiration, invoke *sigev_notify_function*.
&nbsp;
Specifying *sevp* as NULL is equivalent to specifying a pointer to a sigevent structure in which *sigev_notify* is SIGEV_SIGNAL and *sigev_signo* is SIGALRM.

## Returns
- **0**
The function is success, and the ID of the new timer is placed in *timerid*.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EAGAIN**
  The calling process has already created all of the timers it is allowed to.
  &nbsp;
-  **EINVAL**
One of the input parameters is invalid.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
