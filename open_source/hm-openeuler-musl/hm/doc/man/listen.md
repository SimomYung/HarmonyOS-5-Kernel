# listen()
Listen for connections on a socket

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```

## Arguments

- **sockfd**
A file descriptor that refers to a socket of type SOCK\_STREAM or SOCK\_SEQPACKET.
&nbsp;
- **backlog**
The maximum length to which the queue of pending connections for *sockfd* may grow.

## Library
libc

## Description
The listen() function marks a socket to be used to accept incoming connection requests.

## Returns

- **0**
On success
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
- **ENOTSOCK**
The argument *sockfd* is not a socket.
&nbsp;
- **EOPNOTSUPP**
The socket is not of a type that supports the listen() operation.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
