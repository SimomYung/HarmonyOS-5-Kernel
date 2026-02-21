# getcwd()
Get current working directory

## Synopsis
```c
#include <unisted.h>

char* getcwd(char *buf, size_t size);
```
## Arguments

- **buf**
The memory used to store the pathname of the current working directory.
&nbsp;
- **size**
The length of *buf*.

## Library
libc

## Description
The getcwd() function returns a string containing the current working directory of the calling process. If *buf* is NULL, memory for the returned string is obtained with malloc, and can be freed with free.

## Returns

On success, this function returns a string containing the current working directory. On failure, this function returns NULL, and errno is set to indicate the error.

## Errors

- **EINVAL**
The argument size is invalid.
&nbsp;
- **ENOENT**
The return value is not an absolute path name.
&nbsp;
- **ERANGE**
The *buf* is not enough to store the path name.
&nbsp;
- **EFAULT**
The address of *buf* is not valid.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
