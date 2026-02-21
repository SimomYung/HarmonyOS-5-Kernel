# pthread\_cancel()
Send a cancellation request to a thread

## Synopsis
```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
```

## Arguments

- **thread**
Target thread descriptor.

## Library
libc

## Description
The pthread\_cancel() function sends a cancellation request to the target thread specified by *thread*, when and how the target thread acts on this request depends on its cancel state and cancel type.
&nbsp;
Cancel state of a thread can be enabled or disabled, set by pthread\_setcancelstate(). If disabled, then cancellation request will be queued until the cancel state is enabled.
&nbsp;
Cancel type of a thread can be deferred or asynchronous, set by pthread\_setcanceltype(). If deferred, then cancellation request will be delayed until the target thread calls a function that is a cancellation point. In contrast, asynchronous cancel type means the target thread can be canceled at any time, usually immediately.
&nbsp;
When the cancellation request is acted on by the target thread, cancellation cleanup handlers are popped and executed in the reverse order of which they are pushed. In addition, thread-specific data destructors are called. Finally, the target thread exits. Return of pthread\_cancel() does not guarantee that the target thread is canceled. The only way to know whether the target thread has been canceled successfully is to join it using pthread\_join(). If the target thread has been canceled, pthread\_join() will return successfully, and exit status of the target thread is PTHREAD\_CANCELED.
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
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
