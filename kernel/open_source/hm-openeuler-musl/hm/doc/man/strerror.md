# strerror()
Return a string describing the error number

## Synopsis
```c
#include <string.h>

char *strerror(int errnum);
```

## Arguments

- **errnum**
The error number.

## Library
libc

## Description
The strerror() function returns the locale-dependent description (a pointer to a string) of the given error number indicated by *errnum*. If *errnum* is not a valid error number, then "No error information" is returned.

## Returns

- **a string**
The appropriate error description string for *errnum*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
