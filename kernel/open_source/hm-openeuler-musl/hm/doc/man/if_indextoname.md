# if\_indextoname()
Get network interface name of the given index

## Synopsis
```c
#include <net/if.h>

char *if_indextoname(unsigned int ifindex, char *ifname);
```

## Arguments

- **ifindex**
The given network interface index.
&nbsp;
- **ifname**
The buffer to store the target name.

## Library
libc

## Description
The if\_indextoname() function gets the name of the network interface for the given interface index. The name is placed in the buffer pointed to by *ifname*. The buffer size must be greater than the size of network interface name.

## Returns

- **ifname**
Get the target name successfully.
&nbsp;
- **NULL**
An error occurred and errno is set appropriately.

## Errors
- **ENXIO**
No interface found for the index.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
