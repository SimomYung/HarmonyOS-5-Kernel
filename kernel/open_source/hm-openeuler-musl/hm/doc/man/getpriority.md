# getpriority()
Get program scheduling priority

## Synopsis
```c
#include <sys/time.h>
#include <sys/resource.h>

int getpriority(int which, id_t who);
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

## Library
libc

## Description
The function getpriority() can be used to get nice for processes.

## Returns

On success, the new nice value is returned. On error, -1 is returned, and errno is set appropriately.

A successful call can legitimately return -1. To detect an error, set errno to 0 before the call, and check whether it is nonzero after nice() returns -1.

## Errors

- **ESRCH**
No process was located using the which and who values specified.

- **EINVAL**
Process's cnode rref is invalid, or the parameter *which* is PRIO_PGRP or PRIO_USER.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
