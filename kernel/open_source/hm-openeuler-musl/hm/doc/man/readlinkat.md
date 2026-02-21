# readlinkat()
Read resolved target pathname of a symbolic link

## Synopsis

```c
#include <fcntl.h>
#include <unistd.h>

ssize_t readlinkat(int fd, const char *restrict path, char *restrict buf, size_t bufsize);

```

## Arguments
- **fd**
A file descriptor refers to a directory.
&nbsp;
- **path**
The filename of a symbolic link.
If *path* is an empty string, operate on the file referred to by *dfd*.
In this case *dfd* can refer to any type of file, not just a directory.
&nbsp;
- **buf**
The memory used to store the resolved target path.
&nbsp;
- **bufsize**
The size of *buf*.


## Library
libc

## Description
The function readlinkat() reads the resolved target pathname of a symbolic link. readlinkat() does not append a null byte to buf. It will truncate the contents, if the buffer is not enough to hold all of the contents. If *pathname* is absolute, then *fd* is ignored. If *pathname* is relative, and *fd* is the special constant AT_FDCWD, then *pathname* is resolved relative to the current working directory of the calling process.

## Returns
- **positive number**
On success, return the number of bytes stored in buf.
&nbsp;
- **-1**
On error, return -1 and errno is set.

## Errors

- **ENOENT**
The file does not exist.
&nbsp;
- **ENAMETOOLONG**
The *pathname* is too long.
&nbsp;
- **EINVAL**
*bufsize* is not valid or the named file is not a symbolic link.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EFAULT**
The *pathname* is not a valid address.
&nbsp;
- **EBADF**
The *fd* is invalid.
&nbsp;
- **ELOOP**
Too many symbolic links were encountered in resolving the pathname.
&nbsp;
- **ENOTDIR**
A component of the path prefix is not a directory or *pathname* is relative and dirfd is a file descriptor referring to a file other than a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
