# kill()
Send signal to a process

## Synopsis
```c
#include <signal.h>

int kill(pid_t pid, int sig);
```

## Arguments

- **pid**
The process ID of the target process.
&nbsp;
- **sig**
The signum to be sent.

## Library
libc

## Description
The kill() function can be used to send a signal to a process or process group.
&nbsp;
If *pid* is positive, the *sig* will be sent to the process with the specified process ID. If *pid* is 0, the *sig* will be sent to all the processes in the process group which the calling process belongs to. If *pid* is -1, the *sig* will be sent to all the processes which the calling process has permission to send signals. If *pid* is less than -1, the *sig* will be sent to all the processes in the process group whose ID is -*pid*. If *sig* is 0, the *sig* will not be sent really, but this can be used to check if a process or process group exists, or if the caller has permission to send signals.
&nbsp;
The *pid* is not stored in the current system. If you want to get signal info by sigtimedwait or sigwaitinfo, the *pid* will not be returned.
&nbsp;
The signals 32~34 are occupied by musl, try to send 32~34 (by kill, raise, sigqueue, pthread\_kill, pthread\_sigqueue, and so on) may cause undefined behavior.

## Returns

- **0**
At least one signal was sent successfully.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*sig* is invalid.
&nbsp;
- **EPERM**
The calling process has no permission to send the signal to the target process or process group.
&nbsp;
- **ESRCH**
The target process or process group does not exist.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
