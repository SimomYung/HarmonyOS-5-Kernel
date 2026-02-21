# sigqueue()
Queue a signal and data to a process

## Synopsis
```c
#include <signal.h>

int sigqueue(pid_t pid, int sig, const union sigval value);
```

## Arguments

- **pid**
The process ID of the target process.
&nbsp;
- **sig**
The signum to be sent.
&nbsp;
- **value**
The value to be sent with signal.

## Library
libc

## Description
The sigqueue() function can be used to send the signal *sig* to the process *pid*. The permissions required to send a signal are the same as kill(). Send signal 0 can be used to check if the process *pid* exists. The *value* argument is used to specify an accompanying item of data to be sent with the signal, and has the following type:
```c
union sigval {
	int   sival_int;
	void *sival_ptr;
};
```
If the receiving process has installed a handler for this signal using the SA_SIGINFO flag to sigaction(), then it can obtain this data via the *si_value* field of the *siginfo_t* structure passed as the second argument to the handler, the *si_code* field of that structure will be set to SI_QUEUE.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EAGAIN**
Only 32 rt signals are supported, the limit of signals which may be queued has been reached.
&nbsp;
- **EINVAL**
*sig* is invalid.
&nbsp;
- **EPERM**
The calling process has no permission to send the signal to the target process.
&nbsp;
- **ESRCH**
No process has a PID matching *pid*.
&nbsp;
- **EEXIST**
The standard signal has been pending on the thread, but has not yet been processed.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
