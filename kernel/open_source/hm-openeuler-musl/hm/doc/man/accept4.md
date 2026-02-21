# accept4()
Accept a connection on a socket

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

int accept4(int sockfd, struct sockaddr *addr,
	    socklen_t *addrlen, int flags);
```

## Arguments

- **sockfd**
*sockfd* is a socket that has been created with socket().
&nbsp;
- **addr**
*addr* is a pointer to a sockaddr structure.
&nbsp;
- **addrlen**
The *addrlen* argument is a value-result argument: the caller must initialize it to contain the size (in bytes) of the structure pointed to by addr; on return it will contain the actual size of the peer address.
&nbsp;
- **flags**
If *flags* is 0, then accept4() is the same as accept().  The following values can be bitwise ORed in flags to obtain different behavior:
SOCK_NONBLOCK   Set the O_NONBLOCK file status flag on the new open file description.
SOCK_CLOEXEC	Set the close-on-exec(FD_CLOEXEC) flag on the new file descriptor.

## Library
libc

## Description
The accept4() function is used with connection-based socket types (SOCK_STREAM, SOCK_SEQPACKET). It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket.

## Returns

- **nonnegative integer**
On success, these function return a nonnegative integer that is a descriptor for the accepted socket.

- **-1**
On error, -1 is returned, and errno is set appropriately.

## Errors
- **EAGAIN**
The socket is marked nonblocking and no connections are present to be accepted.
&nbsp;
- **EBADF**
The descriptor is invalid.
&nbsp;
- **ECONNABORTED**
A connection has been aborted.
&nbsp;
- **EFAULT**
The addr argument is not in a writable part of the user address space.
&nbsp;
- **EMFILE**
The per-process limit on the number of open file descriptors has been reached.
&nbsp;
- **EOPNOTSUPP**
The referenced socket is not of type SOCK_STREAM.
&nbsp;
- **ENOTSOCK**
Socket operation on non-socket.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
