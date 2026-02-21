# fchmod()
Change permission modes of a file

## Synopsis
```c
#include <sys/stat.h>

int fchmod(int fd, mode_t mode);
```

## Arguments
- **fd**
The descriptor of the file whose permission modes is to be changed.
&nbsp;
- **mode**
The file's new permission modes.

## Library
libc

## Description
The fchmod() function changes user's permission modes of the file pointed to by file descriptor *fd* to *mode*.

If fd is opened by O_PATH, syscall fchmod will fail returning EBADF, but libc will try to use chmod or fchmodat to do chmod by procfs and fd get the path of fd.

## Returns
- **0**
Change the file permission modes successfully.
&nbsp;
- **-1**
When an error occurred, the file permission modes will not change. *-1* will be returned and *errno* is set.

## Errors
- **EBADF**
The file descriptor *fd* is not valid.
&nbsp;
- **EPERM**
The effective user does not own the appropriate privileges.
&nbsp;
- **EROFS**
The file pointed to by *fd* resides on a read-only file system.
&nbsp;
- **EINVAL**
The value of *mode* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

