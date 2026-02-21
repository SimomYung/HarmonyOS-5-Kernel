# getgid()
Get the real group ID

## Synopsis
```c
#include <unistd.h>

gid_t getgid(void);
```

## Arguments
N/A

## Library
libc

## Description

The getgid() function returns the real group ID of the calling process.

## Returns

- **rgid**
The function always succeeds and returns the real group ID of the calling process.

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
