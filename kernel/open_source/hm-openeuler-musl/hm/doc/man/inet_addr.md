# inet\_addr()
Convert the Internet host address from IPv4 numbers-and-dots notation into binary data in network byte order

## Synopsis
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

in_addr_t inet_addr(const char *cp);
```

## Arguments

- **cp**
A string in IPv4 numbers-and-dots notation.

## Library
libc

## Description
The inet_addr() function converts the Internet host address *cp* from IPv4 numbers-and-dots notation into binary data in network byte order.

## Returns

- **The converted address**
Addr is returned successfully.
&nbsp;

- **-1**
An error occurred.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
