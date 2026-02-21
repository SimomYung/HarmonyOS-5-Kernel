# symlink(), symlinkat()
Create a symbolic link for a file

## Synopsis

```c
#include <unistd.h>
int symlink(const char *existing, const char *new);

#include <fcntl.h>
#include <unistd.h>
int symlinkat(const char *existing, int fd, const char *new);
```

## Arguments

- **existing**
  The path name of a existing file or directory.
  &nbsp;
- **new**
  The file name of the symbolic link which will be created by symlink or symlinkat function.
  &nbsp;
- **fd**
  A file descriptor refers to a directory which can be constant AT_FDCWD or other valid value referred to a directory.

## Library
libc

## Description
symlink() creates a new symbolic link for a file named  *existing*. A symbolic link may point to an existing file or to a nonexistent file.If the symbolic link name exists, it will not be overwritten.
&nbsp;
symlinkat() creates a new symbolic link working like symlink(). If the name of the symbolic link is absolute, then the argument *fd* is ignored. If the name of the symbolic link is relative and fd equals constant AT_FDCWD, then the name of the symbolic link is resolved relative to the current working directory of the calling process. If fd doesn't equals constant AT_FDCWD and has refers to a valid directory, then the name of the symbolic link is resolved relative to the directory referred by the *fd*.

## Returns
- **0**
On success, return 0.
&nbsp;
- **-1**
On error, return -1 and errno is set.

## Errors

- **ENOENT**
The argument *existing* or *new* is null string.
&nbsp;
- **ENAMETOOLONG**
The argument *existing* or *new* is too long.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EFAULT**
The argument *existing* or *new* is not a valid address.
&nbsp;
- **EINVAL**
The argument *fd* is not a valid value or the length of *existing* or *new* is not enough.
&nbsp;
- **EBADF**
The argument *fd* is not a valid file descriptor.
&nbsp;
- **EROFS**
The path *new* is on a read-only filesystem.
&nbsp;
- **EEXIST**
The path *new* already exists.
&nbsp;
- **EMLINK**
Too many links exist of the path name *existing*.
&nbsp;
- **EPERM**
Do not support the creation of symbolic links.
&nbsp;
- **EACCES**
Do not permit to create a symbolic links.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
