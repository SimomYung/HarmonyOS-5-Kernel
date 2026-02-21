# recvmsg()
Receive a message from a socket.

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
```

## Arguments

- **sockfd**
The recvmsg() function is normally used only on a connected socket.
&nbsp;
- **msg**
The recvmsg() function uses a msghdr structure to minimize the number of directly supplied arguments. This structure is defined as follows in:
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
The msg_flags field in the msghdr is set on return of recvmsg().  Only support MSG_TRUNC, MSG_CTRUNC.

- **flags**
*flags* is a combination of one or more of the following flags, which can be linked together through the or operation.
Only support MSG_CMSG_CLOEXEC, MSG_WAITALL, MSG_TRUNC, MSG_PEEK.

## Library
libc

## Description
The recvmsg() function is used to receive messages from a socket.

**Ancillary data**
is sent and received using sendmsg and recvmsg, and each sendmsg and recvmsg only supports one auxiliary message types. The auxiliary message types listed below are specified using the SOL_SOCKET type, which are specific to AF_UNIX.
- **SCM_RIGHTS** Send or receive a set of open file descriptors from another process. The data portion contains an integer array of the file descriptors.
    - If the buffer used to receive the ancillary data containing file descriptors is too small (or is absent), then the ancillary data is truncated (or discarded) and the excess file descriptors are automatically closed in the receiving process.
    - SCM_RIGHTS supports the transfer of normal type, device type and socket type file descriptors at the same time.
    - Only support MSG_CMSG_CLOEXEC flags of recvmsg().
## Returns

- **actual bytes**
The recvmsg() function return the number of bytes received.
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
Send has been called before, and the data transmission exceeds the maximum number of retransmissions, but the ACK of the peer has not been received, resulting in the socket being destroyed internally. If recv is called at this time, this error code is returned.
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
