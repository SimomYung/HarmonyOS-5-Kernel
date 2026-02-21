# getuid()
Get a real user ID

## Synopsis
```c
#include <unistd.h>

uid_t getuid(void);
```

## Arguments
N/A

## Library
libc

## Description

The getuid() function returns the real user ID of the calling process. Permission check is not performed before operation.

## Returns

- **uid**
The real user ID queried. The getuid() function is always successful.

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
