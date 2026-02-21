# clock_gettime()
Get the current value of the specified clock

## Synopsis
```c
#include <times.h>;

int clock_gettime(clockid_t clock_id, struct timespec *tp);
```

## Arguments
- **clock_id**
The ID of the particular clock on which to act.
&nbsp;
- **tp**
Pointer to a *timespec* where the current value of the clock is stored.

## Library
libc

## Description
The clock_gettime() function gets the current value of the specified clock by *clock_id*, and stores it in *tp*.
&nbsp;
The following clocks are supported:
- **CLOCK_REALTIME**
System-wide clock that measures real (i.e., wall-clock) time.
&nbsp;
- **CLOCK_REALTIME_COARSE**
A faster but less precise version of CLOCK_REALTIME.
&nbsp;
- **CLOCK_MONOTONIC**
Clock that cannot be set and represents monotonic time since the system starts up.
&nbsp;
- **CLOCK_MONOTONIC_COARSE**
A faster but less precise version of CLOCK_MONOTONIC.
&nbsp;
- **CLOCK_PROCESS_CPUTIME_ID**
Per-process CPU-time clock (measures CPU time consumed by all threads in the process).
&nbsp;
- **CLOCK_THREAD_CPUTIME_ID**
Thread-specific CPU-time clock.
&nbsp;
- **CLOCK_BOOTIME**
Similar to CLOCK_MONOTONIC, but it also includes system suspended time.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINVAL**
The *clock_id* is not supported.
&nbsp;
- **EFAULT**
*tp* points outside the accessible address space.
## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
