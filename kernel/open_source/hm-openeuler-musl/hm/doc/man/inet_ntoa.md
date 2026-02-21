# inet\_ntoa()
Convert the Internet host address in network byte order to a string in IPv4 dotted-decimal notation

## Synopsis
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char *inet_ntoa(struct in_addr in);
```

## Arguments

- **in**
Internet host address in network byte order.

## Library
libc

## Description
The inet_ntoa() function converts the Internet host address in network byte order to a string in IPv4 dotted-decimal notation which will be returned if converted successfully.
The string is returned in a statically allocated buffer, which subsequent calls will overwrite.

## Returns

- **a string in IPv4 dotted-decimal notation**
Addr is converted successfully.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
