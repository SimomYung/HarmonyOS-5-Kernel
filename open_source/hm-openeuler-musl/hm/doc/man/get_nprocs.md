# get_nprocs()
Get the number of available processors

## Synopsis
```c
#include <sys/sysinfo.h>

int get_nprocs(void);
```

## Arguments
N/A

## Library
libc

## Description
Return the number of processors currently available. This may be less than the number of processors physically residing in the system due to offline.

## Returns
Return the number of processors currently available.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
