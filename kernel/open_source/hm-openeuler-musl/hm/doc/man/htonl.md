# htonl()
Convert the unsigned integer from host byte order to network byte order

## Synopsis
```c
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);
```

## Arguments

- **hostlong**
Unsigned integer in host byte order.

## Library
libc

## Description
The htonl() function converts the unsigned integer *hostlong* from host byte order to network byte order.

## Returns
An unsigned integer in network byte order.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
