# ntohl()
Convert the unsigned integer from network byte order to host byte order

## Synopsis
```c
#include <arpa/inet.h>

uint32_t ntohl(uint32_t netlong);
```

## Arguments

- **netlong**
Unsigned integer in network byte order.

## Library
libc

## Description
The ntohl() function converts the unsigned integer from network byte order to host byte order.

## Returns
An unsigned integer in host byte order.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
