# socket()
Create an endpoint for communication

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

## Arguments

- **domain**
The *domain* argument specifies a communication domain; this selects the protocol family which will be used for communication. Only the following domains are supported:
AF_INET, AF_UNIX, AF_NETLINK, AF_TUN, AF_PACKET, AF_CAN, AF_VSOCK.
&nbsp;
- **type**
The socket has the indicated *type*, which specifies the communication semantics.
AF_INET supports SOCK_STREAM, SOCK_DGRAM, SOCK_RAW.
AF_UNIX supports SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET.
AF_NETLINK supports SOCK_DGRAM, SOCK_RAW.
AF_TUN this field is meaningless.
AF_PACKET supports SOCK_DGRAM, SOCK_RAW.
AF_CAN supports SOCK_RAW only.
AF_VSOCK supports SOCK_STREAM only.
The type argument serves a second purpose: in addition to specifying a socket type, it may include the bitwise OR of any of the following values, to modify the behavior of socket():
SOCK_CLOEXEC Set the close-on-exec(FD_CLOEXEC) flag on the new file descriptor.
&nbsp;
- **protocol**
The *protocol* specifies a particular protocol to be used with the socket.
AF_INET SOCK_DGRAM supports IPPROTO_IP, IPPROTO_UDP.
AF_INET SOCK_STREAM supports IPPROTO_IP, IPPROTO_TCP.
AF_INET SOCK_RAW supports IPPROTO_ICMP, IPPROTO_TCP, IPPROTO_UDP.
AF_UNIX supports 0 only.
AF_NETLINK supports NETLINK_ROUTE, NETLINK_GENERIC, NETLINK_KOBJECT_UEVENT, NETLINK_AUDIT.
AF_TUN this field is meaningless.
AF_CAN supports CAN_RAW only.
AF_VSOCK supports 0 only.

## Library
libc

## Description
Socket() creates an endpoint for communication and returns a descriptor.

## Returns

- **0**
On success, zero is returned for the standard options.
&nbsp;
- **-1**
On error, -1 is returned, and errno is set appropriately.

## Errors

- **EAFNOSUPPORT**
The implementation does not support the specified address family.
&nbsp;
- **EINVAL**
Unknown protocol, or protocol family not available. Invalid flags in type.
&nbsp;
- **EMFILE**
The per-process limit on the number of open file descriptors has been reached.
&nbsp;
- **ENFILE**
The system-wide limit on the total number of open files has been reached.
&nbsp;
- **ENOBUFS,ENOMEM**
Insufficient memory is available.  The socket cannot be created until sufficient resources are freed.
&nbsp;
- **EPROTOTYPE**
The socket type is not supported by the protocol.
&nbsp;
- **EPROTONOSUPPORT**
The protocol type or the specified protocol is not supported within this domain.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
