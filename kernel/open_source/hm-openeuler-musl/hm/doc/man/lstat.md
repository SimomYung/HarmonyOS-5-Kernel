# lstat()
Get the status of a file

## Synopsis
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int lstat(const char *restrict path, struct stat *restrict buf);
```

## Arguments
- **path**
The argument *path* refers to the file.
&nbsp;
- **buf**
The status information of a file shall be stored in a stat structure pointed to by *buf*.

## Library
libc

## Description
The function lstat() gets status information of the file pointed to by *path*.

## Returns
- **0**
Get status information of the file successfully.
&nbsp;
- **-1**
When an error occurs, -1 is returned and errno is set.
  
## Errors
- **EACCES**
Looking up the path of the file is denied.
&nbsp;
- **EIO**
An error occurs during reading from the file system.
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
