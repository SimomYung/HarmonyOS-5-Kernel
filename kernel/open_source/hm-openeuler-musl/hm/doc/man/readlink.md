# readlink()
Read resolved target pathname of a symbolic link

## Synopsis

```c
#include <unistd.h>

ssize_t readlink(const char *pathname, char *buf, size_t bufsize);
```

## Arguments

- **pathname**
The filename of a symbolic link.
&nbsp;
- **buf**
The memory used to store the resolved target path.
&nbsp;
- **bufsize**
The size of *buf*.


## Library
libc

## Description
The function readlink() reads the resolved target pathname of a symbolic link. readlink() does not append a null byte to buf. It will truncate the contents, if the buffer is not enough to hold all of the contents.

## Returns

On success, return the number of bytes stored in *buf*.  On error, return -1 and errno is set.

## Errors

- **ENOENT**
The file does not exist.
&nbsp;
- **ENAMETOOLONG**
*pathname* is too long.
&nbsp;
- **EINVAL**
*bufsize* is not valid or the named file is not a symbolic link.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EFAULT**
*pathname* is not a valid address.
&nbsp;
- **EBADF**
Get fd by *pathname* failed.
&nbsp;
- **ELOOP**
Too many symbolic links were encountered in resolving the pathname.
&nbsp;
- **ENOTDIR**
A component of the path prefix is not a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
