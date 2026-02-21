# getegid()
Get the effective group ID

## Synopsis
```c
#include <unistd.h>

gid_t getegid(void);
```

## Arguments
N/A

## Library
libc

## Description

The getegid() function returns the effective group ID of the calling process.

## Returns

- **egid**
The function always succeeds and returns the effective group ID.

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
