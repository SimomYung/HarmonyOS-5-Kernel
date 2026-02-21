# setpriority()
Set program scheduling priority

## Synopsis
```c
#include <sys/time.h>
#include <sys/resource.h>

int setpriority(int which, id_t who, int prio);
```

## Arguments

- **which**
The parameter *which* can be one of the following three values: PRIO_PROCESS, PRIO_PGRP, PRIO_USER. Now, our system only supports the concept of PRIO_PROCESS.
&nbsp;
- **who**
When *which* is PRIO_PROCESS, *who* is the process ID.
&nbsp;
When *which* is PRIO_PGRP, *who* is the group identifier of the process.
&nbsp;
When *which* is PRIO_USER, *who* is the user ID.
&nbsp;
- **prio**
The nice value to set. It's a value in the range -20 to 19, with -20 being the highest priority and 19 being the lowest priority.

Attempts to set a priority outside this range are silently clamped to the range. The default priority is 0.

## Library
libc

## Description
Our system only supports setting the nice value of the ICES scheduling algorithm.
&nbsp;
Our system set the nice of thread with *who*. It is the same with LINUX, but it is different from POSIX.

## Returns

- **0**
Success.
&nbsp;

- **-1**
An error occurred.

## Errors

- **ESRCH**
No process was located using the which and who values specified.

- **EINVAL**
Process's cnode rref is invalid, or the parameter *which* is PRIO_PGRP or PRIO_USER.

- **EACCES**
1)The caller attempted to set a lower nice value (i.e., a higher process priority), but did not have the required privilege.

2)Fail to check Mandatory Access Control.

- **EPERM**
A process was located, but its effective user ID did not match either the effective or the real user ID of the caller, and was not privileged.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
