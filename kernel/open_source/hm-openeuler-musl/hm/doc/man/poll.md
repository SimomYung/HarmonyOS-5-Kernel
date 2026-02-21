# poll(), ppoll()
wait for some event on a file descriptor

## Synopsis
```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t n, int timeout);

#define _GNU_SOURCE         /* See feature_test_macros() */
#include <signal.h>
#include <poll.h>

int ppoll(struct pollfd *fds, nfds_t n,
          const struct timespec *to, const sigset_t *mask);
```

## Arguments

- **fds**
The *fds* is an array of the pollfd structures.
```
struct pollfd {
        int   fd;         /* file descriptor */
        short events;     /* requested events */
        short revents;    /* returned events */
};
```
&nbsp;
- **n**
The number of pollfd in *fds*.
&nbsp;
- **timeout**
The *timeout* specifies the number of milliseconds that poll() should block waiting for a file descriptor to become ready.
&nbsp;
- **to**
Time interval for *ppoll*. *ppoll* will be terminated when time out. It won't be changed by *ppoll*.
&nbsp;
- **mask**
The bit mask to filter the signal when ppoll running.

## Library
libc

## Description
poll() and ppoll() enables a scheme to allow multiplexing I/O on the specified file descriptors. Some supported events are:
- **POLLIN**
There is data to read.
&nbsp;
- **POLLOUT**
Writing is now possible, though a write larger than the available space in a socket or pipe will still block.
&nbsp;
- **POLLERR**
Error condition, only returned in *revents*.
&nbsp;
- **POLLNVAL**
Invalid request, fd in fds is invalid, only returned in *revents*.

See *epoll\_ctl* for more details about supported file descriptors and events. And polling regular file is also supported.

## Returns

- **nonnegative**
Number of elements in the pollfds whose revents fields have been set to a nonzero value (indicating an event or an error). A return value of zero indicates that the system call timed out before any file descriptors became ready.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors

- **EFAULT**
*fds* or *to* is an invalid address.
&nbsp;
- **EBADF**
An invalid file descriptor is given in fds.
&nbsp;
- **EINVAL**
The *n* argument exceeds the RLIMIT\_NOFILE.
*to* is invalid
*mask* is an invalid address.
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
