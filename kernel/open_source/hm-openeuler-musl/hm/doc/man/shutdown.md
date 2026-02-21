# shutdown()
Shuts down all connections to sockets shared by processes.

## Synopsis
```c
#include <sys/socket.h>

int shutdown(int sockfd,int howto);
```

## Arguments

- **sockfd**
*sockfd* is the fd that is being used during communication.
&nbsp;
- **howto**
Different *howto* values correspond to different shutdown behaviors.
*howto* can only be one of the following values: SHUT_WR, SHUT_RD or SHUT_RDWR

## Library
libc

## Description
This interface shuts down all connections to sockets shared by processes.

## Returns

- **0**
On success, zero is returned for the standard options.
&nbsp;
- **-1**
On error, -1 is returned, and errno is set appropriately.

## Errors

- **EAFNOSUPPORT**
Parameter error, the family is not supported or the network function is disabled.
&nbsp;
- **EBADF**
*sockfd* is not a valid descriptor.
&nbsp;
- **ENOTCONN**
*sockfd* is not connected.
&nbsp;
- **EOPNOTSUPP**
When type is non-SOCK_STREAM, shutdown operation is not supported.
&nbsp;
- **EINPROGRESS**
When the shutdown is initiated, the socket is in the write / connect state. In this state, the shutdown operation is not supported.
&nbsp;
- **EINVAL**
*how* argument is invalid.
&nbsp;
- **ENOTSOCK**
*sockfd* is a file descriptor and not a network descriptor.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
