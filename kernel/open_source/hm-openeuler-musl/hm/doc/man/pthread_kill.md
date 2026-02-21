# pthread\_kill()
Send a signal to a thread

## Synopsis
```c
#include <signal.h>

int pthread_kill(pthread_t thread, int sig);
```

## Arguments

- **thread**
The target thread descriptor.
&nbsp;
- **sig**
Signal number.

## Library
libc

## Description
The pthread\_kill() function sends a signal specified by *sig* to the target thread specified by *thread*. This function only guarantees that the signal is delivered to the specified thread, the signal action ("stop", "continue" or "terminate") may affects the whole process.
&nbsp;
If *sig* is zero, error checking is still performed, but no signal action will be triggered.
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
Invalid signal number.
&nbsp;
- **ESRCH**
The thread specified by *thread* is not found.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
