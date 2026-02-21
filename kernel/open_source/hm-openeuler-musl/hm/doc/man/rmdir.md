# rmdir()
Remove a directory

## Synopsis
```c
#include <unistd.h>

int rmdir(const char *path);
```

## Arguments

- **path**
The pathname of the directory specified.
&nbsp;

## Library
libc

## Description

rmdir() removes the directory associated with the argument *path* if the directory is empty.

## Returns
On success, rmdir() returns 0. On error, -1 is returned, and errno is set to indicate the error.

## Errors

- **ENOENT**
The argument *path* is a null string or component of "path" does not exist.
&nbsp;
- **ENAMETOOLONG**
The argument *path* is too long.
&nbsp;
- **EFAULT**
The address of argument *path* is not valid.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **ENOTEMPTY**
The directory is not empty.
&nbsp;
- **ENOTDIR**
The argument *path* is not a directory stream.
&nbsp;
- **EOPNOTSUPP**
The operation is not supported.
&nbsp;
- **EROFS**
Readonly filesystem.
&nbsp;
- **EACCES**
No permission to remove the directory.
&nbsp;
- **EBUSY**
"path" is used as a mount point or is the root directory of the calling process.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
