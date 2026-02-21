# unshare()
Disassociate parts of the process execution context

## Synopsis
```c
#include <sched.h>

int unshare(int flags);
```

## Arguments
- **flags**
A bit mask that specifies which parts of the execution context should be unshared.

## Library
libc

## Description
The unshare() function allows a process (or thread) to disassociate parts of its execution context, specified by *flags*,  that are currently being shared with other processes (or threads).
&nbsp;
Currently supported flags:
*CLONE\_NEWNS* unsharing the mount namespace, so that the calling process has a private copy of its namespace which is not shared with any other process.
*CLONE\_NEWUTS* create new UTS namespace.
*CLONE\_NEWPID* create new PID namespace for child process created by the caller.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINVAL**
An invalid bit was specified in *flags*.
&nbsp;
- **EINVAL**
*CLONE\_NEWPID* is specified but the caller has called unshare with *CLONE\_NEWPID*.
&nbsp;
- **EINVAL**
*CLONE\_NEWUTS* or *CLONE\_NEWPID* is specified but the kernel was not configured *CONFIG\_UTS\_NS* or *CONFIG\_PID\_NS*.

## Example
&nbsp;

## Classification
UNIX

## Function Safety
TODO
