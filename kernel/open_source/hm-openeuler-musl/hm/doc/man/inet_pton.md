# inet\_pton()
Convert the character string src into a network address structure in the af address family

## Synopsis
```c
#include <arpa/inet.h>

int inet_pton(int af, const char *src, void *dst);
```

## Arguments

- **af**
Af address family, only AF\_INET and AF\_INET6 are supported.
&nbsp;
- **src**
The string to be converted.
&nbsp;
- **dst**
Target network address structure.

## Library
libc

## Description
The inet\_ntop() function converts the character string *src* into a network address structure in the *af* address family, then copies the network address structure to *dst*.

## Returns

- **1**
Addr is converted successfully.
&nbsp;
- **0**
*Src* does not contain a character string representing a valid network address in the specified address family.
&nbsp;
- **-1**
Af does not contain a valid address family and errno is set appropriately.

## Errors

- **EAFNOSUPPORT**
Af does not contain a valid address family.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
