# pthread\_join()
Wait for the target thread to exit

## Synopsis
```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

## Arguments

- **thread**
The target thread descriptor.
&nbsp;
- **retval**
A pointer to the buffer which stores the exit status of the target thread.

## Library
libc

## Description
The pthread\_join() function waits for the target thread (should be joinable) specified by *thread* to exit. The exit status of the target thread will be filled in the buffer specified by *retval*. If the target thread does not have an exit status or you do not care about its exit status, you can pass NULL as *retval*.
&nbsp;
On return of pthread\_join(), the target thread should have exited and the resources consumed by the target thread (tid, stack, etc) are recycled. From the moment on, thread descriptor of the target thread cannot be used anymore, or you may cause segmentation fault ( use-after-free ).
&nbsp;
Note that you should pass a valid thread descriptor to this function, invalid *thread* may cause segmentation fault.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
Target thread is detached.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
