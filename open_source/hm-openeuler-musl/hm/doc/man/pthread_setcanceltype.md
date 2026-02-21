# pthread\_setcanceltype()
Set thread cancelability type

## Synopsis
```c
#include <pthread.h>

int pthread_setcanceltype(int type, int *oldtype)
```

## Arguments

- **type**
The new cancelability type set to the calling thread.
&nbsp;
- **oldtype**
The old cancelability type of the calling thread.

## Library
libc

## Description
The pthread\_setcanceltype() function sets the cancelability type of the calling thread to the value given by *type*, and return old cancelability type of the calling thread in the buffer pointed to by *oldtype*. The cancelability type must be one of the following values:
&nbsp;
- **PTHREAD\_CANCEL\_DEFERED**
	The cancellation request will be deferred until the thread calls a function which is a cancellation point or execute a signal handler which contains a cancellation point. This is the default cancelability type of thread.
&nbsp;
- **PTHREAD\_CANCEL\_ASYNCRONOUS**
	The cancellation request will be done at any time.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The value of *type* is invalid.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
