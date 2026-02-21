# epoll\_wait(), epoll\_pwait()
wait for an I/O event on an epoll file descriptor

## Synopsis
```c
#include <sys/epoll.h>

int epoll_wait(int fd, struct epoll_event *ev, int cnt, int to);
int epoll_pwait(int fd, struct epoll_event *ev, int cnt, int to,
                const sigset_t *sigs);
```

## Arguments
- **fd**
The file descriptor of the epoll instance that function (waiting the event) will perform at.
&nbsp;
- **ev**
Buffer, used to return information, for file descriptors in the interest list that have some events available.
&nbsp;
- **cnt**
Number of *struct epoll_events* represented by *ev*. If more than *cnt* fds are ready when epoll\_wait() is called, only *cnt* fds can be stored in *ev*. So it is suggested to have a big enough *ev* to hold all ready fds.
&nbsp;
- **to**
Short for time out, suppose to be greater than zero as well. If *to* is 0, which means the epoll\_wait() will return immediately. If *to* equals -1, which may cause the function to block indefinitely.
&nbsp;
- **sigs**
When sigs equals zero, it's the same with epoll\_wait(). Otherwise, the sigs serves as the signal mask.

## Library
libc

## Description
The epoll\_wait() or epoll\_pwait() system call waits for events on the epoll instance referred to by the file descriptor fd.

## Returns
- **Positive number**
Number of file descriptors ready for the requested I/O.
&nbsp;
- **0**
No fd is ready for the I/O event.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.
&nbsp;

## Errors

- **EFAULT**
*ev* is an invalid address.
&nbsp;
- **EBADF**
*fd* is not a valid file descriptor.
&nbsp;
- **EINVAL**
*fd* is not an epoll file descriptor.
*cnt* is invalid.
*sigs* is an invalid address.
&nbsp;
- **ENOMEM**
No enough memory during the process (unlikely).
&nbsp;
- **EINTR**
A signal occurred before any requested event.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
