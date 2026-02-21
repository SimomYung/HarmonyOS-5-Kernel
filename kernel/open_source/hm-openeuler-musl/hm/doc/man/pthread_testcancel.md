# pthread\_testcancel()
Place a cancellation point to respond cancellation requests

## Synopsis
```c
#include <pthread.h>

void pthread_testcancel(void);
```

## Arguments
N/A

## Library
libc

## Description
For cancellation request whose type is deferred, it will be delayed until the target thread calls a function that is a cancellation point. The pthread\_testcancel() function places a cancellation point in the calling thread, so that the cancellation request pending on the calling thread can be responded.
&nbsp;
If the cancelability state of the calling thread is PTHREAD\_CANCEL\_DISABLE, the pthread\_testcancel() function will have no effect.

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
