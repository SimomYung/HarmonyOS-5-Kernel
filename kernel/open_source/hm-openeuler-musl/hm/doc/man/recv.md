# recv()
Receive a message from a socket.

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

## Arguments

- **sockfd**
The recv() function is normally used only on a connected socket.
&nbsp;
- **buf**
Recv() places the received message into the buffer *buf*.
&nbsp;
- **len**
The caller must specify the size of the buffer in *len*.
&nbsp;
- **flags**
The *flags* argument is formed by logically OR’ing zero or more of the following values:
MSG_DONTWAIT, MSG_PEEK, MSG_TRUNC, MSG_WAITALL.

## Library
libc

## Description
The recv() function is used to receive messages from a socket.

## Returns

- **actual bytes**
The recv() function return the number of bytes received.
&nbsp;
- **0**
On success, zero is returned for the standard options.
&nbsp;
- **-1**
On error, -1 is returned, and errno is set appropriately.

## Errors

- **EAFNOSUPPORT**
Parameter error, the family is not supported or the network function is disabled.
&nbsp;
- **EAGAIN,EWOULDBLOCK**
The socket is marked nonblocking and the receive operation would block, or a receive timeout had been set and the timeout expired before data was received.
&nbsp;
- **EBADF**
The argument sockfd is an invalid descriptor.
&nbsp;
- **EFAULT**
The receive buffer pointer(s) point outside the process's address space.
&nbsp;
- **EINVAL**
Invalid argument passed.
&nbsp;
- **ECONNREFUSED**
Send has been called before, and the data transmission exceeds the maximum number of retransmissions, but the ACK of the peer has not been received, resulting in the socket being destroyed internally. If recv() is called at this time, this error code is returned.
&nbsp;
- **ENOTCONN**
The socket is associated with a connection-oriented protocol and has not been connected.
&nbsp;
- **EIO**
IO error.
&nbsp;
- **ENOTSOCK**
The file descriptor sockfd does not refer to a socket.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
