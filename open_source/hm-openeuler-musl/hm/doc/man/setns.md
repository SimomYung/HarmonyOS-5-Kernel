# setns()
Associate the caller thread with a specified namespace

## Synopsis
```c
#include <sched.h>

int setns(int fd, int nstype);
```

## Arguments

- **fd**
The file descriptor of namespace.
&nbsp;
- **nstype**
The type of the namespace.

## Library
libc

## Description
Function setns() re-directs caller to a namespace (specified by the given *fd*). Currently, *CLONE_NEWNS*, *CLONE_NEWUTS* and *CLONE_NEWPID* are supported.

## Returns

- **0**
Function finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors

- **EBADF**
The input *fd* is invalid.
&nbsp;
- **EINVAL**
The input *nstype* is not supported.
&nbsp;
- **ENOMEM**
There is not enough memory during the function (unlikely to happen).
&nbsp;
- **EINVAL**
The caller tried to join an ancestor PID namespace.
&nbsp;
- **EINVAL**
*CLONE\_NEWUTS* or *CLONE\_NEWPID* is specified but the kernel was not configured *CONFIG\_UTS\_NS* or *CONFIG\_PID\_NS*.

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
