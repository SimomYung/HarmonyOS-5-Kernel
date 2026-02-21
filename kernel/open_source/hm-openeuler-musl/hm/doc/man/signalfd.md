# signalfd()
Register handler for signal

## Synopsis
```c
#include <sys/signalfd.h>

int signalfd(int fd, const sigset_t *mask, int flags);
```

## Arguments

- **fd**
A file descriptor created by signalfd function that can be used to accept signals targeted at the caller.
&nbsp;
- **mask**
The mask argument specifies the set of signals that the caller wishes to accept via the file descriptor.
&nbsp;
- **flags**
Flags of *fd*

## Library
libc

## Description
The *signalfd()* function creates a file descriptor that can be used to accept signals targeted at the caller. This provides an alternative to the use of a signal handler or sigwaitinfo, and has the advantage that the file descriptor may be monitored by select, poll, and epoll.
&nbsp;
This *mask* argument is a signal set whose contents can be initialized using the macros described in sigsetops. Normally, the set of signals to be received via the file descriptor should be blocked using sigprocmask, to prevent the signals being handled according to their default dispositions.It is not possible to receive SIGKILL or SIGSTOP signals via a signalfd file descriptor; these signals are silently ignored if specified in mask.
&nbsp;
If the *fd* argument is -1, then the call creates a new file descriptor and associates the signal set specified in mask with that file descriptor. If fd is not -1, then it must specify a valid existing signalfd file descriptor, and mask is used to replace the signal set associated with that file descriptor.

## Returns

On success, *signalfd()* returns a signalfd file descriptor; this is either a new file descriptor (if fd was -1), or *fd* if *fd* was a valid signalfd file descriptor. On error, -1 is returned and errno is set to indicate the error.

## Errors

- **EBADF**
The *fd* file descriptor is not a valid file descriptor.

- **EINVAL**
*fd* is not a valid signalfd file descriptor.

- **EINVAL**
flags is invalid;

- **EMFILE**
The per-process limit of open file descriptors has been reached.

- **ENOMEM**
There was insufficient memory to create a new signalfd file descriptor.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SIGNALFD_SUPPORT off. Calling the API under this condition will return ENOSYS.
