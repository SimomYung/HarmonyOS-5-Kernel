# inet\_ntop()
Convert the network address structure src in the af address family into a character string

## Synopsis
```c
#include <arpa/inet.h>

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
```

## Arguments

- **af**
Af address family, only AF\_INET and AF\_INET6 are supported.
&nbsp;
- **src**
A network address structure to be converted.
&nbsp;
- **dst**
Target buffer to store the converted string.
&nbsp;
- **size**
Size of the target buffer.

## Library
libc

## Description
The inet\_ntop() function converts the network address structure *src* in the *af* address family into a character string. The resulting string is copied to the buffer pointed to by *dst*, which must be a non-NULL pointer. The caller specifies the number of bytes available in this buffer in the argument *size*.

## Returns

- **a non-NULL pointer to dst**
Addr is converted successfully.
&nbsp;
- **NULL**
There is an error and errno is set appropriately; *size* is negative or *size* bigger than INT\_MAX.

## Errors

- **EAFNOSUPPORT**
Af does not contain a valid address family.
&nbsp;
- **ENOSPC**
The converted address string would exceed the size given by *size*; *size* is negative or larger than INT\_MAX.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
