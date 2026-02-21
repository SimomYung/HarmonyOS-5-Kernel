# freeaddrinfo()

Free the entire linked list returned by getaddrinfo()

## Synopsis

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void freeaddrinfo(struct addrinfo *res);
```

## Arguments

- **res**
  The *res* is the linked list of struct addrinfos returned by getaddrinfo().

## Library

libc

## Description

It frees the linked list of struct addrinfo returned by getaddrinfo().

## Returns

N/A

## Errors

N/A

## Example

&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
