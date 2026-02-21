# freeifaddrs()

Free the entire linked list returned by getifaddrs()

## Synopsis

```c
#include <sys/types.h>
#include <ifaddrs.h>

void freeifaddrs(struct ifaddrs *ifa);
```

## Arguments

- **ifa**
  It's a pointer points to a listed list of struct ifaddrs returned by getifaddrs().

## Library

libc

## Description

Free the linked list of struct ifaddrs returned by getifaddrs().

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
