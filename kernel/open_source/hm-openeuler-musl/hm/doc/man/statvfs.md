# statvfs(), statvfs64()
Get a filesystem statics

## Synopsis
```c
#include <sys/statvfs.h>

int statvfs(const char *path, struct statvfs *buf);
int statvfs64(const char *path, struct statvfs *buf);
```

## Arguments 
- **path**
The argument *path* refers to a file in the mounted filesystem.
&nbsp;
- **buf**
The argument *buf* is a pointer to a *statvfs* structure.

## Library
libc

## Description
The function statvfs() and statvfs64() get the information (f_bsize, f_blocks, f_bfree, f_bavail, f_namemax) of a mounted filesystem.

## Returns
- **0**
Get statics of the filesystem successfully.
&nbsp;
- **-1**
When an error occurs, -1 is returned and the errno is set.

## Errors
- **EACCES**
Search permission of the component of *path* is forbidden.
&nbsp;
- **EFAULT**
The argument *path* is invalid.
&nbsp;
- **EIO**
While reading from a filesystem, an I/O error occurs.
&nbsp;
- **ELOOP**
Too many symbolic links are encountered while translating path.
&nbsp;
- **ENAMETOOLONG**
The argument *path* is greater than *{PATH_MAX}*.
&nbsp;
- **ENOENT**
The file *path* refers to does not exist.
&nbsp;
- **ENOMEM**
The memory is insufficient.
&nbsp;
- **ENOSYS**
This call is not supported in the filesystem.
&nbsp;
- **ENOTDIR**
The prefix component of *path* referred to a file other than a directory.
&nbsp;
- **EOVERFLOW**
Values returned are larger than *statvfs* structure.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
