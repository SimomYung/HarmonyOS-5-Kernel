# getsockopt()
Get option value of a specific socket

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
```

## Arguments

- **sockfd**
A socket given by user.
&nbsp;
- **level**
Specifies the protocol level at which the option resides. For a inet socket, set the *level* to SOL\_SOCKET, IPPROTO\_TCP, IPPROTO\_IP to get options at the sockets API level, TCP level and IP level respectively; for a unix socket, Set the *level* to SOL\_SOCKET; for a packet socket, set the *level* to SOL\_PACKET; for a netlink socket, set the *level* to SOL\_SOCKET; for a vsock socket, *level* is meaningless. Other sockets and levels are not supported now.
&nbsp;
- **optname**
For a inet socket, specify the option\_name argument as SO\_REUSEPORT, SO\_BROADCAST, SO\_KEEPALIVE, SO\_MARK, SO\_COOKIE, SO\_RCVBUF, SO\_RCVTIMEO, SO\_SNDBUF, SO\_SNDTIMEO, SO\_LINGER, SO\_ERROR, SO\_DOMAIN, SO\_PROTOCOL to get options at socket level; specify the option\_name argument as TCP\_NODELAY, TCP\_KEEPIDLE, TCP\_KEEPINTVL, TCP\_KEEPCNT, TCP\_INFO to get options at TCP level; specify the option\_name argument as IP\_TOS、IP\_TTL、IP\_PKTINFO、IPV6\_PKTINFO to get options at IP level; specify the option\_name argument as IPT\_SO\_GET\_INFO, IPT\_SO\_GET\_ENTRIEST, IPT\_SO\_GET\_REVISION\_MATCH, IPT\_SO\_GET\_REVISION\_TARGET for iptables tool.
For a unix socket, only SO\_REUSEADDR, SO\_ERROR, SO\_PASSCRED, SO\_PEERSEC, SO\_DOMAIN, SO\_SNDBUF, SO\_RCVBUF, SO\_RCVTIMEO, SO\_SNDTIMEO, SO\_PROTOCOL, SO\_MARK, SO\_COOKIE, SO\_LINGER, SO\_PEERCRED at SOL\_SOCKET level are supported.
For a packet socket, PACKET\_STATISTICS at SOL\_PACKET level and SO\_REUSEADDR, SO\_ERROR, SO\_PASSCRED, SO\_PEERSEC, SO\_DOMAIN, SO\_SNDBUF, SO\_RCVBUF, SO\_RCVTIMEO, SO\_SNDTIMEO, SO\_PROTOCOL, SO\_MARK, SO\_COOKIE, SO\_LINGER at SOL\_SOCKET level are supported.
For a netlink socket, SO\_REUSEADDR, SO\_ERROR, SO\_PASSCRED, SO\_PEERSEC, SO\_DOMAIN, SO\_SNDBUF, SO\_RCVBUF, SO\_RCVTIMEO, SO\_SNDTIMEO, SO\_PROTOCOL, SO\_MARK, SO\_COOKIE, SO\_LINGER at SOL\_SOCKET level is supported.
For a vsock socket, VSOCK\_OPT\_BUFFER\_SIZE is supported.
&nbsp;
- **optval**
A buffer to store option value.
&nbsp;
- **optlen**
Optlen is initialized to contain *size* of the buffer pointed to by *optval*, and modified on return to indicate the actual size of the value returned.

## Library
libc

## Description
The getsockopt() function gets the option value of the socket referred to by the file descriptor *sockfd* at different level.

## Returns

- **0**
Get the option value successfully.
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
If *optlen* is not in a valid part of the process address space.
&nbsp;
- **ENOPROTOOPT**
The option is unknown at the *level* indicated or the *level* is not SOL\_SOCKET, SOL\_PACKET, IPPROTO\_TCP or IPPROTO\_IP.
&nbsp;
- **ENOTSOCK**
The argument *sockfd* is a file, not a socket.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
