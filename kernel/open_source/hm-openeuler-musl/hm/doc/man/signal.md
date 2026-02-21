# signal()
Register handler for signal

## Synopsis
```c
#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);
```

## Arguments

- **signum**
The signal to be registered.
&nbsp;
- **handler**
The disposition of the signal.

## Library
libc

## Description
The signal() function sets the disposition of the *signum* to *handler*, which is either SIG_IGN, SIG_DFL, or the address of a programmer-defined function.
&nbsp;
If the disposition is set to SIG_IGN, the signal will be ignored. If the disposition is set to SIG_DFL, the default action associated with the signal will occur. If the disposition is set to a function, the function will be called when the process get the signal *signum*.
&nbsp;
The signals SIGKILL and SIGSTOP cannot be caught or ignored.
&nbsp;
The signal() is equivalent to calling sigaction() with the following flags: sa.sa_flags = SA_RESTART;

## Returns

signal() returns the previous signal handler on success, or SIG_ERR on error.

## Errors

- **EINVAL**
*signum* is invalid.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
