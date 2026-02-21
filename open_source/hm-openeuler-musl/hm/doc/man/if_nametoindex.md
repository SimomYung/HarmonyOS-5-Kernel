# if\_nametoindex()
Get the index of a given network interface name

## Synopsis
```c
#include <net/if.h>

unsigned int if_nametoindex(const char *ifname);
```

## Arguments

- **ifname**
The given network interface name.

## Library
libc

## Description
The if\_nametoindex() function returns the index of the network interface corresponding to the name *ifname*.

## Returns

- **the index number**
Get the index successfully.
&nbsp;

- **0**
An error occurred.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
