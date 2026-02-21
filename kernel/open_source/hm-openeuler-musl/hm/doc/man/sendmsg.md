# sendmsg()
Send a message on a socket.

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
```

## Arguments

- **sockfd**
The argument sockfd is the file descriptor of the sending socket.
&nbsp;
- **msg**
The address of the target is given by msg.msg_name, with msg.msg_namelen specifying its size. The definition of the msghdr structure employed by sendmsg() is as follows:
```c
	   struct msghdr {
               void         *msg_name;       /* optional address */
               socklen_t     msg_namelen;    /* size of address */
               struct iovec *msg_iov;        /* scatter/gather array */
               size_t        msg_iovlen;     /* # elements in msg_iov */
               void         *msg_control;    /* ancillary data, see below */
               size_t        msg_controllen; /* ancillary data buffer len */
               int           msg_flags;      /* flags (unused) */
           };
```

- **flags**
The *flags* argument is the bitwise OR of zero or more of the following flags:
    - **MSG_DONTWAIT**
Enables nonblocking operation; if the operation would block, EAGAIN or EWOULDBLOCK is returned.
    - **MSG_MORE**
Callers has more data to send, only works with TCP sockets. As to UDP and RAW sockets, it takes no effect.
    - **MSG_NOSIGNAL**
Don't generate a SIGPIPE signal if the peer on a stream-oriented socket has closed the connection.
    - **MSG_DONTROUTE**
Don't  use a gateway to send out the packet, send to hosts only on directly connected networks. Only valid on RAW and UDP sockets.
    - **MSG_CONFIRM**
Tell the link layer We got a successful reply from remote. Only SOCK_DGRAM and SOCK_RAW sockets support this.

## Library
libc

## Description
The function sendmsg() is used to transmit a message to another socket.

**Ancillary data**
is sent and received using sendmsg and recvmsg, and each sendmsg and recvmsg only supports one auxiliary message types. The auxiliary message types listed below are specified using the SOL_SOCKET type, which are specific to AF_UNIX.
- **SCM_RIGHTS** Send or receive a set of open file descriptors from another process. The data portion contains an integer array of the file descriptors.
    - SCM_RIGHTS supports the transfer of normal type, device type and socket type file descriptors at the same time.
## Returns

- **actual bytes**
On success, the sendmsg() function return the number of bytes sent.
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
- **EMFILE**
The number of "in-flight" file descriptors exceeds the limits when sending a file descriptor as ancillary data over a UNIX domain socket (see the description of SCM_RIGHTS, above). An "in-flight" file descriptor is one that has been sent using sendmsg but has not yet been accepted in the recipient process using recvmsg.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
