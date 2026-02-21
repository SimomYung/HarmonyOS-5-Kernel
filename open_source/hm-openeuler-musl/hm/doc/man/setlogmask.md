# setlogmask()
Set mask to log

## Synopsis
```c
#include <syslog.h>

int setlogmask(int maskpri);
```

## Arguments

- **maskpri**
The mask value will be set to log. If *maskpri* is 0, the function will not do anything.

## Library
libc

## Description
Function setlogmask() sets the given logmask (from *maskpri*).


## Returns
Function returns the previous mask.

## Errors
N/A

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
