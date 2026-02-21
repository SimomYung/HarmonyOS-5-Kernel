# fdopen()
Open a FILE stream base on fd

## Synopsis
```c
#include <fcntl.h>
#include <stdio.h>

FILE *fdopen(int fd, const char *mode);
```

## Arguments

- **fd**
The file descriptor relates to the FILE.
&nbsp;
- **mode**
The mode will define different kinds of operations on FILE.

## Library
libc

## Description
The fdopen() function obtains a FILE stream based on the inputted *fd*. Different operations can be performed according to the *mode* parameter. For example, mode *r* indicates to read the FILE, *w* indicates to write FILE, and *a* indicates to operate the FILE with append mode.

## Returns

- **FILE**
When fdopen() works successfully.
&nbsp;
- **0**
When the *mode* is not one of *a*, *w*, or *r*.
&nbsp;
- **NULL**
Operation failed since unexpected situations happened, such as the fd is not a valid file descriptor.
&nbsp;


## Errors

- **EINVAL**
When the *mode* parameter is not valid, fdopen() will return 0.
&nbsp;
- **EBADF**
Bad file descriptor and fdopen() will return NULL.
&nbsp;


## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
