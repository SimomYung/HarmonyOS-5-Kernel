# stat()
Get the status information of a file

## Synopsis
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat(const char *restrict path, struct stat *restrict buf);
```

## Arguments 
- **path**
The argument *path* refers to a file.
&nbsp;
- **buf**
The stat structure information shall be stored in *buf*.

## Library
libc

## Description
The function stat() gets status information (st_nlink, st_rdev, st_size, st_blksize, st_blocks, st_dev, st_ino, st_mode) of the file pointed to by *path* and st_mode is 0777 by default.

## Returns
- **0**
Get status information of the file successfully.
&nbsp;
- **-1**
When an error occurs, -1 is returned and errno is set.

## Errors
- **EACCES**
The prefix of *path* is denied for searching.
&nbsp;
- **EFAULT**
The *path* is invalid.
&nbsp;
- **ELOOP**
A loop exists in symbolic links during resolution of *path*.
&nbsp;
- **ENAMETOOLONG**
The length of *path* is longer than *{NAME_MAX}*.
&nbsp;
- **ENOENT**
The *path* is an empty string or it does not point to an existing file.
&nbsp;
- **ENOMEM**
The memory is insufficient.
&nbsp;
- **ENOTDIR**
The prefix of *path* refers to an existing file that is neither a directory nor a symbolic link to a directory.
&nbsp;
- **EOVERFLOW**
One of the values obtained overflows the member of *stat* structure.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
