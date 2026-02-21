# getchar()
Get a byte from the stdin stream

## Synopsis
```c
#include <stdio.h>

int getchar(void);
```

## Arguments
N/A

## Library
libc

## Description
The getchar() function obtains the next byte as an unsigned char converted to an int from the stdin stream.

## Returns
Get a byte successfully, the next byte from the stdin stream is returned.

If the end-of-file indicator for the stdin stream is set or it is at end-of-file, *EOF* is returned.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

