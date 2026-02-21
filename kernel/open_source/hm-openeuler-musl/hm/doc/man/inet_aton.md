# inet\_aton()
Convert the Internet host address from the IPv4 numbers-and-dots notation into binary form (in network byte order)

## Synopsis
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int inet_aton(const char *cp, struct in_addr *inp);
```

## Arguments

- **cp**
A string in IPv4 numbers-and-dots notation.
  &nbsp;
- **inp**
An in\_addr structure to store converted address.

## Library
libc

## Description
inet\_aton() returns 1 if the supplied string was successfully interpreted, or 0 if the string is invalid (errno is not set on error).

## Returns

- **1**
Addr is converted successfully.
&nbsp;
- **0**
An error occurred.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
