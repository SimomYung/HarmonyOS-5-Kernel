# connect()
Initiate a connection on a socket.

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr,
	    socklen_t addrlen);
```

## Arguments

- **sockfd**
*sockfd* is a socket that has been created with socket().
&nbsp;
- **addr**
*addr* is a pointer to a sockaddr structure.
&nbsp;
- **addrlen**
*addrlen* specifies the size.

## Library
libc

## Description
The connect() function connects the socket referred to by the file descriptor sockfd to the address specified by addr.

## Returns

- **0**
If the connection or binding succeeds, zero is returned.
&nbsp;

- **-1**
On error, -1 is returned, and errno is set appropriately.

## Errors

- **EAFNOSUPPORT**
Parameter error, the family is not supported or the network function is disabled.
&nbsp;
- **EADDRINUSE**
Local address is already in use.
&nbsp;
- **EALREADY**
The socket is nonblocking and a previous connection attempt has not yet been completed.
&nbsp;
- **EBADF**
The file descriptor is not a valid index in the descriptor table.
&nbsp;
- **ECONNREFUSED**
No-one listening on the remote address.
&nbsp;
- **EFAULT**
The socket structure address is outside the user's address space.
&nbsp;
- **EINPROGRESS**
The  socket  is  nonblocking  and the connection cannot be completed immediately.
&nbsp;
- **EISCONN**
The socket is already connected.
&nbsp;
- **ENETUNREACH**
Network is unreachable.
&nbsp;
- **ENOTSOCK**
The file descriptor sockfd does not refer to a socket.
&nbsp;
- **EPROTOTYPE**
The socket type does not support the requested communications protocol.
&nbsp;
- **ECONNRESET**
The internal state of the socket is abnormal, and the normal sending and receiving functions have been lost.
&nbsp;
- **ENOTCONN**
The internal state of the socket is abnormal. The resources of the socket have been partially destroyed.
&nbsp;
- **EADDRNOTAVAIL**
Destination address connection already exists.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
