# pthread\_sigqueue()
Queue a signal and accompanied data to a thread

## Synopsis
```c
#define _GNU_SOURCE
#include <signal.h>
#include <pthread.h>

int pthread_sigqueue(pthread_t thread, int sig, const union sigval value);
```

## Arguments

- **thread**
The thread descriptor to which queue a signal and accompanied data.
&nbsp;
- **sig**
The signal number queued to the thread.
&nbsp;
- **value**
The accompanied data to be queued to the thread.

## Library
libc

## Description
The pthread\_sigqueue() function can send a signal accompanied by data to the *thread* in the same process as the calling thread. The *sig* specifies the signal number sent to the *thread* and the *value* specifies accompanied data. The argument *value* has following type:
&nbsp;
```c
union sigval {
    int    sival_int;
    void  *sival_ptr;
};
```
&nbsp;
The target thread can obtain *value* via the *si_value* field of the argument *siginfo_t* of the signal handler if the signal handler is installed with SA\_SIGINFO flag, and the *si_code* field of the argument *siginfo_t* will be set to SI\_QUEUE.
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The signal number is invalid.
&nbsp;
- **EAGAIN**
The queued signals have exceeded the maximum number.
&nbsp;
- **ESRCH**
The thread specified by *thread* is not found.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
