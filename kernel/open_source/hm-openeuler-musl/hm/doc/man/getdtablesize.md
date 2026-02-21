# getdtablesize()
Get descriptor table size

## Synopsis
```c
#include <unistd.h>

int getdtablesize(void);
```

## Arguments
N/A

## Library
libc

## Description
The getdtablesize() function returns the maximum number of files a process can open.

## Returns
The maximum number of open files per process.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
