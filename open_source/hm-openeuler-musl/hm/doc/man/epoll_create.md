# epoll\_create(), epoll\_create1()
Create an epoll instance and return its fd (file descriptor)

## Synopsis
```c
#include <sys/epoll.h>

int epoll_create(int size);
int epoll_create1(int flags);
```

## Arguments

- **size**
The size argument is ignored, but must be greater than zero.
- **flags**
If  *flags* is 0, then, epoll\_create1() is the same as epoll\_create() that ignores the *size* argument.
If  *flags* is EPOLL\_CLOEXEC, it will set close-on-exec flag on the new fd.
If  *flags* is the other values, -1 will returned, and an errno of EINVAL will be setted.

## Library
libc

## Description
The epoll\_create() function creates and returns the fd of the created epoll instance.
Other than the fact that the obsolete size argument is dropped, the epoll\_create() is same as epoll\_create1() with flags equals to 0.

## Returns
- **fd**
The function completes successfully and return the fd.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.


## Errors
- **EINVAL**
*size* is not a positive integer
The flags is neither 0 nor EPOLL\_CLOEXEC
&nbsp;
- **ENOMEM**
No enough memory during the process (unlikely).
&nbsp;
- **EMFILE**
The per-process limit on the number of open file descriptors has been reached.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
