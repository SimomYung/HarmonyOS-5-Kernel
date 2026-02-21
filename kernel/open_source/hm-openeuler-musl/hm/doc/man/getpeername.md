# getpeername()

Return address info about the remote side of the connection

## Synopsis

```c
#include <sys/socket.h>

int getpeername(int s, struct sockaddr *addr, socklen_t *len);
```

## Arguments

- **s**
  Socket descriptor.
  &nbsp;
- **addr**
  A pointer to a caller-allocated buffer used to place peer address.
  &nbsp;
- **len**
  A pointer to a caller-allocated buffer used to place length information. It should be initialized to indicate the amount of space pointed to by *addr*. On return it contains the actual size of the name returned (in bytes). The name is truncated if the buffer provided is too small. In this case, *len* will return a value greater than was supplied to the call.

## Library

libc

## Description

getpeername() simply returns a struct sockaddr_in filled with information about the machine you're connected to. There are a lot of different kinds of sockets, In Internet Sockets case, though, the peer's "name" is it's IP address and port. Although the function returns the size of the resultant address in *len*, you must preload *len* with the size of *addr*.

## Returns

Returns zero on success, or -1 on error (and *errno* will be set accordingly).

## Errors

The variable *errno* can have the following values:

- **EAFNOSUPPORT**
  Parameter error, the family is not supported or the network function is disabled.
  &nbsp;
- **EBADF**
  The argument *s* is not a valid descriptor.
  &nbsp;
- **EFAULT**
  The *addr* argument points to memory not in a valid part of the process address space.
  &nbsp;
- **EINVAL**
  *len* is invalid (e.g., is negative).
  &nbsp;
- **ENOBUFS**
  Insufficient resources were available in the system to perform the operation.
  &nbsp;
- **ENOTCONN**
  The socket is not connected.
  &nbsp;
- **ENOTSOCK**
  The file descriptor *s* does not refer to a socket.

## Example

```C
#include <stdio.h>
#include <sys/socket.h>

int main(int *argc, char **argv)
{
    /* assume s is a connected socket */
    socklen_t len;
    struct sockaddr_storage addr;
    char ipstr[INET6_ADDRSTRLEN];
    int port;

    len = sizeof addr;
    getpeername(s, (struct sockaddr*)&addr, &len);

    /* deal with both IPv4 and IPv6: */
    if (addr.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
    } else { /* AF_INET6 */
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
        port = ntohs(s->sin6_port);
        inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
    }

    printf("Peer IP address: %s\n", ipstr);
    printf("Peer port      : %d\n", port);
    return 0;
}
```

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
