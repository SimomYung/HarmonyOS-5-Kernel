# clock_adjtime()
Display or set system clock variables

## Synopsis
```c
#include <sys/timex.h>;

int clock_adjtime(clockid_t clk_id, struct timex *buf);
```

## Arguments
- **clock_id**
The ID of the particular clock on which to act. Only CLOCK_REALTIME is currently supported.
&nbsp;
- **buf**
Pointer to a *timex* structure, which is used to reset system parameters from selected fields.

## Library
libc

## Description
The clock_adjtime() function tunes system clock and returns the partial system parameters through
the structure *timex*. The structure is specified in <timex.h>:

```c
struct timex {
	int  modes;      /* Mode selector */
	long offset;     /* Time offset; when STA_NANO flag is set, its unit is nanoseconds,
			    otherwise, its unit is microseconds */
	long freq;       /* Frequency offset */
	long maxerror;   /* Maximum error (microseconds), it is not supported currently */
	long esterror;   /* Estimated error (microseconds), it is not supported currently */
	int  status;     /* Clock command/status */
	long constant;   /* PLL (phase-locked loop) time constant */
	long precision;  /* Clock precision (microseconds, read-only) */
	long tolerance;  /* Clock frequency tolerance (read-only) */
	struct timeval time;
			 /* Current time (read-only, except for ADJ_SETOFFSET); upon return,
			    when STA_NANO flag is set, the unit of time.tv_usec is nanoseconds,
			    otherwise its microseconds */
	long tick;       /* Microseconds between clock ticks */
	long ppsfreq;    /* PPS (pulse per second) frequency (read-only) */
	long jitter;     /* PPS jitter (read-only), it is not supported currently */
	int  shift;      /* PPS interval duration, it is not supported currently */
	long stabil;     /* PPS stability (read-only); it is not supported currently */
	long jitcnt;     /* PPS count of jitter limit exceeded events (read-only), it is not
			    supported currently */
	long calcnt;     /* PPS count of calibration intervals (read-only), it is not supported
			    currently */
	long errcnt;     /* PPS count of calibration errors (read-only), it is not supported
			    currently */
	long stbcnt;     /* PPS count of stability limit exceeded events (read-only), it is not
			    supported currently */
	int tai;         /* TAI offset, it is not supported currently */
};
```

The *modes* field currently supports one or more of the following bits:

- **ADJ_FREQUENCY**
	Set ntp frequency offset according to *buf.freq*. Its value is limited to (-3276800, 3276800).

- **ADJ_STATUS**
	Set ntp clock status bits according to *buf.status*.

- **ADJ_TIMECONST**
	Set PLL time constant according to *buf.constant*. If the status **STA_NANO** is cleared,
	**ADJ_TIMECONST** will add 4.

- **ADJ_SETOFFSET**
	Add *buf.time* to the current realtime. If *buf.status* includes the **ADJ_NANO** flag, then
	the unit of *buf.time.tv_usec* is set to nanoseconds, otherwise it is set to microseconds.

	The *buf.time* has two fields, where the field *buf.time.tv_usec* must always be nonnegative.

- **ADJ_MICRO**
	Select microsecond resolution.

- **ADJ_NANO**
	Select nanosecond resolution. Where **ADJ_MICRO** and **ADJ_NANO** can only select one of them.

- **ADJ_TICK**
	Set ntp tick value according to *buf.tick*.

- **ADJ_OFFSET_SINGLESHOT**
	Adjust system time according to *buf.offset*, which specifies an adjustment in microseconds.

The *status* field currently supports **STA_NANO** for resolution (0 = microsecond, 1 = nanoseconds).
Set via **ADJ_NANO**, cleared via **ADJ_MICRO**.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINVAL**
Failed to get data from vdso.
*buf* is an invalid pointer.

## Example
&nbsp;
## Classification
Unix

## Function Safety
TODO
