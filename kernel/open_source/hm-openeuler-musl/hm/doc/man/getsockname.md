# getsockname()
Get the local address and port to which a socket is bound

## Synopsis
```c
#include <sys/socket.h>

int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

## Arguments

- **sockfd**
A socket specified by users.
&nbsp;
- **addr**
The buffer to store the returned address.
&nbsp;
- **addrlen**
Indicates the amount of space of *addr* and will be updated to contain the actual size of *addr* when the function returns.

## Library
libc

## Description
The getsockname() function is used to get the local address and port to which a socket is bound.

## Returns

- **0**
Addr is returned successfully.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EAFNOSUPPORT**
Parameter error, the family is not supported or the network function is disabled.
&nbsp;
- **EBADF**
The argument *sockfd* is not a valid descriptor.
&nbsp;
- **EFAULT**
The memory address of *addr* is not in a valid part of the process address space.
&nbsp;
- **EINVAL**
*Addrlen* is invalid.
&nbsp;
- **ENOBUFS**
Insufficient resources were available in the system to perform the operation.
&nbsp;
- **ENOTSOCK**
The argument *sockfd* is a file, not a socket.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
