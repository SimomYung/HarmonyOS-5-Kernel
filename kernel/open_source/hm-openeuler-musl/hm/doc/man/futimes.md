# futimes()
Change the file timestamps

## Synopsis
```c
#include <sys/time.h>

int futimes(int fd, const struct timeval tv[2]);
```

## Arguments 
- **fd**
The file descriptor refers to a file.
&nbsp;
- **tv[2]**
The argument *tv[2]* array is used to store the last access and modification time.

## Library
libc

## Description
The futimes() function changes the access and modification time of the file.

## Returns
- **0**
Change the file timestamps successfully, 0 is returned.
&nbsp;
- **-1**
An error occurs, -1 is returned and *errno* is set.

## Errors
- **EACCES**
The argument *tv[2]* is NULL, the caller does not have the write permission to the file.
&nbsp;
- **EPERM**
The argument *tv[2]* is not NULL, the caller does not have the write permission to the file.
&nbsp;
- **EROFS**
The argument *fd* resides on a read-only filesystem.
&nbsp;
- **EBADF**
The argument *fd* is not a valid file descriptor.
&nbsp;
- **ENOSYS**
The */proc* filesystem can not be accessed.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
