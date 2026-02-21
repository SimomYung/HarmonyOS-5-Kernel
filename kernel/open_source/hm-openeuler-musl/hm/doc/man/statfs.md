# statfs()
Get the filesystem statistics

## Synopsis
```c
#include <sys/vfs.h>

int statfs(const char *path, struct statfs *buf);
```

## Arguments 
- **path**
The argument *path* refers to a file in the filesystem.
&nbsp;
- **buf**
The argument *buf* is a pointer to a *statfs* structure.

## Library
libc

## Description
The statfs() function returns some information about a mounted filesystem referred by *path* which can not be a pipe for now. The information includes these fields of struct statf: sf_bsize, f_blocks, f_bfree, f_bavail, f_namemax, f_type.

## Returns
- **0**
Get the filesystem statistics successfully, 0 is returned.
&nbsp;
- **-1**
An error occurs, -1 is returned and *errno* is set.

## Errors
- **EACCES**
A component of the path prefix of *path* is denied for search permission.
&nbsp;
- **EFAULT**
The argument *buf* or *path* points to an invalid address.
&nbsp;
- **EIO**
An I/O error occurs while reading from the filesystem.
&nbsp;
- **ELOOP**
Too many symbolic links are encountered in translating *path*.
&nbsp;
- **ENAMETOOLONG**
The length of *path* is too long.
&nbsp;
- **ENOENT**
The file pointed to by *path* does not exist.
&nbsp;
- **ENOMEM**
Insufficient memory is available.
&nbsp;
- **ENOSYS**
The call is not supported in the filesystem.
&nbsp;
- **EOVERFLOW**
Some values are too large to represented in the returned *statfs* structure.
&nbsp;
- **ENOTDIR**
A component of the path prefix of *path* does not refer to a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
