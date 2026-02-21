# geteuid()
Get the effective user ID

## Synopsis
```c
#include <unistd.h>

uid_t geteuid(void);
```

## Arguments
N/A

## Library
libc

## Description

The geteuid() function returns the effective user ID of the calling process.

## Returns

- **euid**
The function always succeeds and the effective user ID is returned.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_DAC off, Calling the API under this condition will return 0 and have no effect.
