# sched\_rr\_get\_interval()
Get round-robin time slice of the target process

## Synopsis
```c
#include <sched.h>

int sched_rr_get_interval(pid_t pid, struct timespec *tp);
```

## Arguments

- **pid**
Target process's id, if *pid* is 0, then calling process will be the target.
&nbsp;
- **tp**
Time slice of the target process.


## Library
libc

## Description
The sched\_rr\_get\_interval() function gets the target process's round-robin time slice. The target process's scheduling policy should be SCHED\_RR.

## Returns

- **0**
Get the target process's round-robin time slice successfully.
&nbsp;
- **-1**
Failed to get time slice of the target process.


## Errors

- **EFAULT**
Write query result to *tp* failed. It is the same with LINUX, but it is different from POSIX.
&nbsp;
- **EINVAL**
*pid* is invalid. It is the same with LINUX, but it is different from POSIX.
&nbsp;
- **ESRCH**
Target process not found.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
