# access(), faccessat()
Check user's permissions for a file

## Synopsis

```c
#include <unistd.h>
int access(const char *filename, int amode);

#include <fcntl.h>
#include <unistd.h>
int faccessat(int fd, const char *filename, int amode, int flag);
```

## Arguments

- **filename**
  The path name of a directory or a file.
  &nbsp;
- **amode**
  A mask consisting of the bitwise OR of one or more of R_OK, W_OK, F_OK, and X_OK.
  &nbsp;
- **fd**
  A file descriptor refers to a directory.
  &nbsp;
- **flag**
  A mask consisting of the bitwise OR of one or more of 0, AT_EACCESS, and AT_SYMLINK_NOFOLLOW.
## Library
libc

## Description
access() checks whether the calling process can access the file *filename*. If *filename* is a symbolic link, it is dereferenced. The amode specifies the accessibility to be performed, and is either the value F_OK, or a mask consisting of the bitwise OR of one or more of R_OK, W_OK, and X_OK. F_OK tests for the existence of the file. R_OK, W_OK, and X_OK test whether the file exists and grants read, write, and execute permissions, respectively.
faccessat() operates in exactly the same way as access(), except for some differences here. If *filename* is absolute, then *fd* is ignored. If *filename* is relative, then it is interpreted relative to the directory referred to by *fd*. If *filename* is relative and fd equals the special value AT_FDCWD, then *filename* is interpreted relative to the current working directory of the calling process.
*flag* is constructed by ORing together zero or more of the following values:
- **AT_EACCESS**
This flag checks using the effective user and group IDs. By default, faccessat() uses the real IDs.
&nbsp;
- **AT_SYMLINK_NOFOLLOW**
This flag is currently not supported.

## Returns
- **0**
On success, return 0.
&nbsp;
- **-1**
On error, return -1 and errno is set appropriately.

## Errors

- **ENOENT**
The argument *filename* is null string or *filename* does not exist.
&nbsp;
- **ENAMETOOLONG**
The argument *filename* is too long.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EBADF**
The argument *fd* is not a valid file descriptor.
&nbsp;
- **EROFS**
The path *filename* is on a read-only filesystem.
&nbsp;
- **EACCES**
Do not permit to access the file.
&nbsp;
- **EINVAL**
The argument *amode* or *flag* is invalid or *filename* is a empty string.
&nbsp;
- **ENOTDIR**
A component used as a directory in *filename* is not a directroy.
&nbsp;
- **ELOOP**
Too many symbolic links were encountered in resolving *filename*.
&nbsp;
- **EOPNOTSUPP**
The operation is not supported.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
