# fstatfs()
Get the filesystem statistics

## Synopsis
```c
#include <sys/vfs.h>

int fstatfs(int fd, struct statfs *buf);
```

## Arguments 
- **fd**
The file descriptor refers to an open file of a mounted filesystem.
&nbsp;
- **buf**
The argument *buf* is a pointer to a *statfs* structure.

## Library
libc

## Description
The fstatfs() function returns some information about a mounted filesystem referred by *fd* which can not be a pipe for now. The information includes these fields of struct statf: sf_bsize, f_blocks, f_bfree, f_bavail, f_namemax, f_type.

## Returns
- **0**
Get the filesystem statistics successfully, 0 is returned. 
&nbsp;
- **-1**
An error occurs, -1 is returned and *errno* is set.

## Errors
- **EBADF**
The argument *fd* is not a valid open file descriptor.
&nbsp;
- **EFAULT**
The argument *buf* points to an invalid address.
&nbsp;
- **EIO**
An I/O error occurs while reading from the filesystem.
&nbsp;
- **ENOMEM**
Insufficient memory is available.
&nbsp;
- **ENOSYS**
The call is not supported in the filesystem.
&nbsp;
- **EOVERFLOW**
Some values are too large to represented in the returned *statfs* structure.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
