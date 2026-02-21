# fstat()
Get the status information of a file

## Synopsis
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int fstat(int fd, struct stat *statbuf);
```

## Arguments 
- **fd**
The file descriptor refers to a file.
&nbsp;
- **statbuf**
The stat structure information is stored in *statbuf*.

## Library
libc

## Description
The function fstat() gets status information (st_nlink, st_rdev, st_size, st_blksize, st_blocks, st_dev, st_ino, st_mode) of the file pointed to by *fd* and st_mode is 0777 by default.

## Returns
- **0**
Get status information of the file successfully.
&nbsp;
- **-1**
When an error occurs, -1 is returned and *errno* is set.

## Errors
- **EBADF**
The argument *fd* is not a valid file descriptor.
&nbsp;
- **EIO**
An I/O error occurs when reading form the filesystem.
&nbsp;
- **EOVERFLOW**
The values obtained overflow the size of *stat* structure.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
