# htons()
Convert the unsigned short integer from host byte order to network byte order

## Synopsis
```c
#include <arpa/inet.h>

uint16_t htons(uint16_t hostshort);
```

## Arguments

- **hostshort**
Unsigned short integer in host byte order.

## Library
libc

## Description
The htons() function converts the unsigned short integer *hostshort* from host byte order to network byte order.

## Returns
An unsigned short integer in network byte order.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
