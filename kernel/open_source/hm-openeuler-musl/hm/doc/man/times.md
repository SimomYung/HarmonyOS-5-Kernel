# times()
Get process times

## Synopsis
```c
#include <sys/times.h>;

clock_t times(struct tms *buffer);
```

## Arguments
- **buffer**
Points to the structure where the time-accounting information is stored. The tms structure is defined in &lt;sys/times.h&gt;

## Library
libc

## Description
The times() function gets the current process times and returns it in structure *buffer*. The tms_utime member of struct tms member is the CPU time spent executing instructions of the calling process, in our system it also includes the CPU time spent in the system while executing tasks on behalf of the calling process, which is different from the posix definition.
&nbsp;
The tms\_stime, tms\_cutime, tms\_cstime members are not supported.

## Returns
The value returned is the elapsed real time, in clock ticks, since system starts up.
&nbsp;
- **(clock\_t) -1**
An error occurred.

## Errors
N/A

## Example

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
