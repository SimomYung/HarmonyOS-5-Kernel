# clock_getres()
Get the resolution of the specified clock

## Synopsis
```c
#include <times.h>;

int clock_getres(clockid_t clock_id, struct timespec *res);
```

## Arguments
- **clock_id**
The ID of the particular clock on which to act.
&nbsp;
- **res**
Pointer to a *timespec* where the resolution of the clock is stored.

## Library
libc

## Description
The clock_getres() function gets the resolution of the specified clock by *clock_id*, and stores it in *res*.
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

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINVAL**
The *clock_id* is not supported.
*res* is NULL.
&nbsp;
- **EFAULT**
*res* points outside the accessible address space.
## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
