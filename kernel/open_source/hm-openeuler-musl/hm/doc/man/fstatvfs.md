# fstatvfs(), fstatvfs64()
Get a filesystem statics

## Synopsis
```c
#include <sys/statvfs.h>

int fstatvfs(int fd, struct statvfs *buf);
int fstatvfs64(int fd, struct statvfs *buf);
```

## Arguments
- **fd**
The file descriptor refers to a file.
&nbsp;
- **buf**
The parameter *buf* is a pointer to a *statvfs* structure.

## Library
libc

## Description
The function fstatvfs() and fstatvfs64() get the information of a mounted filesystem.

## Returns
- **0**
Get statics of the filesystem successfully.
&nbsp;
- **-1**
When an error occurs, -1 is returned and the errno is set.

## Errors
- **EBADF**
*fd* is an invalid open file descriptor.
&nbsp;
- **EFAULT**
*buf* points to an invalid address.
&nbsp;
- **EIO**
An I/O error occurred during reading from the filesystem.
&nbsp;
- **ENOMEM**
Insufficient memory is available.
&nbsp;
- **ENOSYS**
The filesystem does not support the call.
&nbsp;
- **EOVERFLOW**
A value is too large to store in the statvfs structure.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
