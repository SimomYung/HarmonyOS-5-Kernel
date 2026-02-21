# telldir()
Obtain the offset of directory

## Synopsis
```c
#include <dirent.h>

long telldir(DIR *dir);
```

## Arguments

- **dir**
The name of the directory that will be operated.

## Library
libc

## Description
The telldir() function will return the current location (offset) of directory *dir*.

## Returns
- **long integer**
The current location of *dir*.
&nbsp;
- **-1**
Error occurred, function failed since the *dir* is invalid.

## Errors
- **EBADF**
The parameter *dir* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
