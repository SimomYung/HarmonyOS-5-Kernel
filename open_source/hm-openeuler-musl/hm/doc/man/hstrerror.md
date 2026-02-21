# hstrerror()
Take an error number and return the corresponding message string

## Synopsis
```c
#include <netdb.h>

const char *hstrerror(int err);
```

## Arguments

- **err**
The given error index.

## Library
libc

## Description
The hstrerror() function is used to get the error message string associated with the error code returned from network functions.

## Returns

- **string**
The string corresponding to the error code.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
