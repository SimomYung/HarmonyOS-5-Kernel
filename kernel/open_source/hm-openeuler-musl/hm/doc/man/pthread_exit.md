# pthread\_exit()
Terminate the calling thread

## Synopsis
```c
#include <pthread.h>

void pthread_exit(void *retval);
```

## Arguments

- **retval**
Exit status.

## Library
libc

## Description
The pthread\_exit() function terminates the calling thread, any codes behind this call will not be executed. *retval* is regarded as the exit status of the calling thread, and it can be obtained by a successful join of this thread.
&nbsp;
Any cancellation cleanup handlers that have been pushed but not yet been popped will be popped and executed. Besides, if the thread has any thread-specific data, the corresponding destructors will be called.
&nbsp;
An implicit call to pthread\_exit() is made when a thread other than main thread first invokes return from the start routine that is used to create it. The function's return value will be regarded as the thread's exit status.

## Returns

N/A

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
