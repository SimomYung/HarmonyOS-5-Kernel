# realpath()
Obtain the real absolute pathname

## Synopsis
```c
#include <stdlib.h>
#include <limits.h>

char *realpath(const char *restrict filename, char *restrict resolved);
```

## Arguments

- **filename**
The source file name or path.
&nbsp;
- **resolved**
The target buffer to store the real absolute pathname.

## Library
libc
&nbsp;

## Description
Function realpath() returns the canonicalised absolute pathname of *filename* and stores it in *resolved*.
&nbsp;

## Returns

- **char\**
Function finished successfully, will return the pointer of the resolved path.
&nbsp;
- **NULL**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors
- **EINVAL**
The input *filename* is NULL or invalid (cannot be looked up in system).
&nbsp;
- **EBADF**
Can not find file descriptor (fd) corresponding to *filename*.
&nbsp;
- **ENOMEM**
There is not enough memory during the function (unlikely to happen).
&nbsp;
- **ENAMETOOLONG**
*filename* exceeded NAME_MAX, or an entire *filename* exceeded PATH_MAX characters.
&nbsp;
- **ENOENT**
The named file does not exist.
&nbsp;
- **ENOTDIR**
A component of the *filename* is not a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
