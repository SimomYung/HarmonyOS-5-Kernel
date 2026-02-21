# ctermid()
Get controlling terminal name

## Synopsis
```c
#include <stdio.h>

char *ctermid(char *s);
```

## Arguments

- **s**
The pointer points to a buffer used to hold the terminal pathname.


## Library
libc

## Description
The ctermid() function returns a string which is the pathname for the current controlling terminal for this process.

## Returns

The pointer to the pathname.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
