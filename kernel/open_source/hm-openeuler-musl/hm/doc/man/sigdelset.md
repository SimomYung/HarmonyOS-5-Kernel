# sigdelset()
Delete a signal from a signal set

## Synopsis
```c
#include <signal.h>

int sigdelset(sigset_t *set, int signo);
```

## Arguments

- **set**
The target signal set.
&nbsp;
- **signo**
The signal to be deleted from the signal set.

## Library
libc

## Description
The sigdelset() function deletes the individual signal specified by the *signo* from the signal set which the *set* points to.
&nbsp;
The signals 32~34 are not currently supported. Try to delete 32~34 from *set* is not feasible, it will return -1.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*signo* is invalid.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
