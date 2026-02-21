# pthread\_setcancelstate()
Set thread cancelability state

## Synopsis
```c
#include <pthread.h>

int pthread_setcancelstate(int state, int *oldstate);
```

## Arguments

- **state**
The new cancelability state set to the calling thread.
&nbsp;
- **oldstate**
The old cancelability state of the calling thread.

## Library
libc

## Description
The pthread\_setcancelstate() function sets the cancelability state of calling thread to the value given by *state*, and return old cancelability state of calling thread in the buffer pointed to by *oldstate*. The cancelability state must be one of the following values:
&nbsp;
- **PTHREAD\_CANCEL\_ENABLE**
	The thread is cancelable. This is the default cancelability state of thread.
&nbsp;
- **PTHREAD\_CANCEL\_DISABLE**
	The thread is not cancelable. The received cancellation will be blocked until the thread is set PTHREAD\_CANCEL\_ENABLE.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The value of *state* is invalid.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
