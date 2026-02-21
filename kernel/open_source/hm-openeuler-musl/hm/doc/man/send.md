# send()
Send a message on a socket.

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

## Arguments

- **sockfd**
The argument sockfd is the file descriptor of the sending socket.
&nbsp;
- **buf**
The message is found in *buf*.
&nbsp;
- **len**
The message has length *len*.
&nbsp;
- **flags**
The *flags* argument is the bitwise OR of zero or more of the following flags:
    - **MSG_DONTWAIT**
Enables nonblocking operation; if the operation would block, EAGAIN or EWOULDBLOCK is returned.
    - **MSG_MORE**
Callers has more data to send, only works with TCP sockets. As to UDP and RAW sockets, it takes no effect.
    - **MSG_NOSIGNAL**
Don't generate a SIGPIPE signal if the peer on a stream-oriented socket has closed the connection.
    - **MSG_CONFIRM**
Tell the link layer We got a successful reply from remote. Only SOCK_DGRAM and SOCK_RAW sockets support this.

## Library
libc

## Description
The system function send() is used to transmit a message to another socket.

## Returns

- **actual bytes**
On success, the send() function return the number of bytes sent.
&nbsp;

- **-1**
On error, -1 is returned, and errno is set appropriately.

## Errors

- **EAFNOSUPPORT**
Parameter error, the family is not supported or the network function is disabled.
&nbsp;
- **EAGAIN,EWOULDBLOCK**
The socket is marked nonblocking and the requested operation would block.
&nbsp;
- **EBADF**
An invalid descriptor was specified.
&nbsp;
- **ECONNRESET**
Socket internal resources have been destroyed.
&nbsp;
- **EFAULT**
An invalid user space address was specified for an argument.
&nbsp;
- **EINVAL**
Invalid argument passed.
&nbsp;
- **EMSGSIZE**
The socket type requires that message be sent atomically, and the size of the message to be sent made this impossible.
&nbsp;
- **ENOBUFS**
The output queue for a network interface was full.
&nbsp;
- **ENOMEM**
No memory available.
&nbsp;
- **ENOTCONN**
The socket is not connected, and no target has been given.
&nbsp;
- **ENOTSOCK**
The file descriptor sockfd does not refer to a socket.
&nbsp;
- **EHOSTUNREACH**
Network is unreachable.
&nbsp;
- **ECONNABORTED**
Connection has been dropped.
&nbsp;
- **EIO**
IO error.
&nbsp;
- **ETIMEDOUT**
Send has been called before, and the data transmission exceeds the maximum number of retransmissions, but the ACK of the peer has not been received, resulting in the socket being destroyed internally. If send is called again at this time, this error code is returned.
&nbsp;
- **EPIPE**
The local end has been shut down on a connection oriented socket. In this case, the process will also receive a SIGPIPE unless MSG_NOSIGNAL is set.
- **MSG_DONTROUTE**
Don't  use a gateway to send out the packet, send to hosts only on directly connected networks. Only valid on RAW and UDP sockets.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
