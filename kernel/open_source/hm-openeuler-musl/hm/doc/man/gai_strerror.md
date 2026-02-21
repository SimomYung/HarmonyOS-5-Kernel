# gai_strerror()

Convert error code to an error message

## Synopsis

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

const char *gai_strerror(int errcode);
```

## Arguments

- **errcode**
  The non-zero value returned by getaddrinfo function.

## Library

libc

## Description

The gai_strerror() function returns a text string describing an error value for the getaddrinfo() and getnameinfo() functions.

## Returns

Pointer to a string describing what the *errcode* means.

## Errors

N/A

## Example

&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
