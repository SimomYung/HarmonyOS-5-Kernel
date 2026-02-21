# setsockopt()
Set options on sockets

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

int setsockopt(int sockfd, int level, int optname,
	       const void *optval, socklen_t optlen);
```

## Arguments

- **sockfd**
*sockfd* is a socket that has been created with socket(), The socket to be set.
&nbsp;
- **level**
The protocol layer where the option is located, now we support following levels.
AF_INET socket supports SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP.
AF_UNIX socket supports SOL_SOCKET.
AF_NETLINK socket supports SOL_SOCKET.
AF_PACKET socket supports SOL_SOCKET.
AF_VSOCK socket level is meaningless.
&nbsp;
- **optname**
The option name to be accessed, now we support following optname.
AF_INET socket level SOL_SOCKET supports SO_REUSEADDR, SO_BROADCAST, SO_KEEPALIVE, SO_SNDBUF, SO_RCVBUF, SO_RCVTIMEO, SO_SNDTIMEO, SO_LINGER, SO_MARK.
AF_INET socket level IPPROTO_IP supports IP_TOS, IP_TTL, IP_MULTICAST_TTL, IP_MULTICAST_IF, IP_MULTICAST_LOOP, IP_ADD_MEMBERSHIP, IP_DROP_MEMBERSHIP.
AF_INET socket level IPPROTO_TCP supports TCP_NODELAY, TCP_KEEPALIVE, TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT.
AF_INET socket level SOL_IP supports IPT_SO_SET_REPLACE, IPT_SO_SET_ADD_COUNTERS.
AF_UNIX socket level SOL_SOCKET supports SO_REUSEADDR, SO_PASSCRED, SO_SNDBUF, SO_RCVBUF, SO_RCVTIMEO, SO_SNDTIMEO, SO_ATTACH_FILTER, SO_DETACH_FILTER, SO_MARK, SO_LINGER.
AF_NETLINK socket level SOL_SOCKET supports SO_REUSEADDR, SO_PASSCRED, SO_SNDBUF, SO_RCVBUF,, SO_RCVTIMEO, SO_SNDTIMEO, SO_ATTACH_FILTER, SO_DETACH_FILTER, SO_MARK, SO_LINGER.
AF_NETLINK socket level SOL_NETLINK supports NETLINK_ADD_MEMBERSHIP, NETLINK_DROP_MEMBERSHIP, NETLINK_CAP_ACK, NETLINK_EXT_ACK.
AF_PACKET socket level SOL_SOCKET supports SO_REUSEADDR,SO_PASSCRED, SO_SNDBUF, SO_RCVBUF, SO_RCVTIMEO, SO_SNDTIMEO, SO_ATTACH_FILTER, SO_DETACH_FILTER, SO_MARK, SO_LINGER.
AF_VSOCK socket supports VSOCK_OPT_BUFFER_SIZE.
&nbsp;
- **optval**
Points to the buffer containing the new option value.
&nbsp;
- **optlen**
The length of the current option.

## Library
libc

## Description
Setsockopt() manipulate options for the socket referred to by the file descriptor sockfd.
Only for an inet socket, the max value(32767s) of l_linger set for SO_LINGER is smaller than that in linux and if the value given is greater than the max, it will be set to the max.

## Returns

- **0**
On success, zero is returned for the standard options.
&nbsp;
- **-1**
On error, -1 is returned, and errno is set appropriately.

## Errors

- **EBADF**
The argument sockfd is not a valid descriptor.
&nbsp;
- **EFAULT**
The address pointed to by optval is not in a valid part of the process address space.
&nbsp;
- **EINVAL**
Optlen invalid in setsockopt().  In some cases this error can also occur for an invalid value in optval.
&nbsp;
- **ENOPROTOOPT**
The option is unknown at the level indicated.
&nbsp;
- **EDOM**
*optval* / *optlen* is incorrect when optname is SO_RCVTIMEO / SO_SNDTIMEO.
&nbsp;
- **ENOMEM**
Internal memory error.
&nbsp;
- **EADDRNOTAVAIL**
Setting failed when optname is IP_DROP_MEMBERSHIP.
&nbsp;
- **EPERM**
The caller has no permission (**POSIX_CAP_NET_ADMIN**) to do the operation when *level* is set to SOL_SOCKET and *optname* is set to SO_MARK.
&nbsp;
- **ENOTSOCK**
Socket operation on non-socket.
## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
