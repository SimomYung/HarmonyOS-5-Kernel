# timerfd_create(), timerfd_settime(), timerfd_gettime()
Timers that notify via file descriptors

## Synopsis
```c
#include <sys/timerfd.h>

int timerfd_create(int clockid, int flags);

int timerfd_settime(int fd, int flags,
		    const struct itimerspec *new_value,
		    struct itimerspec *old_value);

int timerfd_gettime(int fd,  struct itimerspec *curr_value);
```

## Arguments
- **clockid**
The clock that the new timer uses to measure time.
&nbsp;
- **flags**
This parameter specifies different types of behavior of function, such as:
TFD\_CLOEXEC, set the close-on-exec behavior on the fd (returned by the function).
TFD\_NONBLOCK, set the O\_NONBLOCK status on the returned fd.
&nbsp;
- **fd**
The file descriptor fd of the timer.
&nbsp;
- **new_value**
The new\_value argument specifies the initial expiration and interval for the timer.
&nbsp;
- **old_value**
The old\_value is used to return the setting of the timer that was current at the time of call when it is not NULL.
&nbsp;
- **curr_value**
The curr\_value contains the current settime of the timer referred to by the file descriptor fd.
&nbsp;

## Library
libc

## Description
These functions create and operate on a timer that delivers timer expiration notifications via a file descriptor. They provide an alternative to the use of setitimer or timer\_create, with the advantage that the file descriptor may be monitored by select, poll, and epoll. The use of these three syscall calls is analogous to the use of timer\_create, timer\_settime, and timer\_gettime.
&nbsp;
For *clockid*, only CLOCK\_MONOTONIC, CLOCK\_REALTIME and CLOCK\_BOOTTIME are supported.
&nbsp;
- **timerfd_settime**
For *flags*, only TFD\_TIMER\_CANCEL\_ON\_SET is supported.
&nbsp;

TFD\_TIMER\_CANCEL\_ON\_SET
This flag should be specified along with TFD\_TIMER\_ABSTIME and the clock for this timer is CLOCK\_REALTIME, then mark this timer as cancelable if the real-time clock undergoes a discontinuous change (settimeofday(2), clock\_settime(2), or similar). When such changes occur, a current or future read(2) from the file descriptor will fail with the error ECANCELED.
&nbsp;

## Returns

- **timerfd_create**
On success, timerfd\_create() returns a new file descriptor. On error, -1 is returned and errno is set to indicate the error.
&nbsp;
- **timerfd_settime**
On success, timerfd\_settime() returns 0. On error, -1 is returned and errno is set to indicate the error.
&nbsp;
- **timerfd_gettime**
On success, timerfd\_gettime() returns 0. On error, -1 is returned and errno is set to indicate the error.
&nbsp;

## Errors
- **EFAULT**
  The *new_value*, *old_value*, or *curr_value* is not a valid pointer.
  &nbsp;
- **EBADF**
  The *fd* is not a valid file descriptor.
  &nbsp;
- **EINVAL**
  One of the input parameters is invalid.

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
