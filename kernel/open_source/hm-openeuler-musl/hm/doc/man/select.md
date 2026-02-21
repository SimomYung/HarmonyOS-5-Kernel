# select(), pselect()
synchronous I/O multiplexing

## Synopsis
```c
#include <sys/select.h>

int select(int n, fd_set *restrict rfds,
           fd_set *restrict wfds, fd_set *restrict efds,
           struct timeval *restrict tv);
int pselect(int n, fd_set *restrict rfds,
            fd_set *restrict wfds, fd_set *restrict efds,
            const struct timespec *restrict ts,
            const sigset_t *restrict mask);
```

## Arguments

- **n**
The maximum number of file descriptor that will be checked during *select*. If *n* is larger than FD\_SETSIZE, the excessive fds will be ignored.
&nbsp;
- **rfds**
Pointer to the fd\_set object where stores fd(s) of file(s) ready for reading.
&nbsp;
- **wfds**
Pointer to the fd\_set object where stores fd(s) of file(s) ready for writing.
&nbsp;
- **efds**
Pointer to the fd\_set object where stores fd(s) of file(s) who has error conditions diagnosing.
&nbsp;
- **tv**
Time interval for *select*. *select* will be terminated when time out. It won't be changed by *select*.
&nbsp;
- **ts**
Parameter for *pselect*, similar to *tv* explained above.
&nbsp;
- **mask**
*mask* is a pointer to a signal mask. If *mask* is not NULL, *pselect* will replace the current signal mask by *mask*, then perform *select* function, and resotre the original signal mask at last.

## Library
libc

## Description
select() and pselect() are very similar, they check the file descriptors (given at *rfds*, *wfds*, *efds*) and obtain their status (i.e., ready for reading, writing, or having an exceptional condition pending).
&nbsp;
See *epoll\_ctl* for more details about supported file descriptors. And selecting regular file is also supported.


## Returns

- **nonnegative**
number of file descriptors contained in the three returned descriptor sets (that is, the total number of bits that are set in *rfds*, *wfds*, *efds*). The return value may be zero if the time interval expired or interrupted by signal before any file descriptors became ready.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors

- **EFAULT**
Either *rfds*, *wfds*, *efds*, *tv* or *ts* is an invalid address.
&nbsp;
- **EBADF**
An invalid file descriptor is given in one of the fd\_sets.
&nbsp;
- **EINVAL**
*n* is invalid.
*tv* or *ts* is invalid.
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
