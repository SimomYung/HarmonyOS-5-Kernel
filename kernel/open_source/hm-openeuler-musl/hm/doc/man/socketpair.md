# socketpair()
Create a pair of connected sockets.

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

int socketpair(int domain, int type, int protocol, int sv[2]);
```

## Arguments

- **domain**
The *domain* argument specifies a communication domain; this selects the protocol family which will be used for communication. We only support AF_UNIX now.
&nbsp;
- **type**
The socket has the indicated *type*, which specifies the communication semantics, the same to socket() api.
&nbsp;
- **protocol**
The *protocol* specifies a particular protocol to be used with the socket, the same to socket() api.
&nbsp;
- **sv[2]**
The descriptors used in referencing the new sockets are returned in sv[0] and sv[1].  The two sockets are indistinguishable.

## Library
libc

## Description
The socketpair() function creates an unnamed pair of connected sockets in the specified domain, of the specified type, and using the optionally specified protocol.

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
&nbsp;
- **EFAULT**
The address sv does not specify a valid part of the process address space.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
