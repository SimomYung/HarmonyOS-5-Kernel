# ntohs()
Convert the unsigned short integer from network byte order to host byte order

## Synopsis
```c
#include <arpa/inet.h>

uint16_t ntohs(uint16_t netshort);
```

## Arguments

- **netshort**
Unsigned short integer in network byte order.

## Library
libc

## Description
The ntohs() function converts the unsigned short integer from network byte order to host byte order.

## Returns
An unsigned short integer in host byte order.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
