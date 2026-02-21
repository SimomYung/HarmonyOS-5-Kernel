# unlink(), unlinkat()
Delete a name and possibly the file it refers to

## Synopsis

```c
#include <unistd.h>
int unlink(const char *path);

#include <fcntl.h>
#include <unistd.h>
int unlinkat(int fd, const char *path, int flag);
```

## Arguments

- **path**
The path name of a directory or file.
&nbsp;
- **fd**
A file descriptor refer to a directory.
&nbsp;
- **flag**
If *flag* is 0, unlinkat() performs the same as unlink() on path. If the AT\_REMOVEDIR flag is specified, then performs the same as rmdir() on pathname.

## Library
libc

## Description
unlink() deletes a name from the filesystem. If the name was the last link to a file and no processes have open the file, the file is deleted and the space it was using can be reused.If the name was the last link to a file but any processes still have the file open, the file will remain in existence until the last file descriptor referring to it is closed. If the path referred to a symbolic link, the symbolic link is removed.
&nbsp;
unlinkat() function runs the same way as unlink(). If the *path* is absolute, then fd is ignored. If the *path* given is relative, then it is resolved relative to the directory referred to by the file descriptor fd. If the *path* given is relative and fd equals constant AT\_FDCWD, then path is resolved relative to the current working directory of the calling process. The argument flag can either be zero, or AT\_REMOVEDIR(linking rmdir() on path).

## Returns
- **0**
On success, return 0.
&nbsp;
- **-1**
On error, return -1 and errno is set.

## Errors
- **ENOENT**
The argument *path* is a null string.
&nbsp;
- **ENAMETOOLONG**
The argument *path* is too long.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EFAULT**
The argument *path* is not a valid address.
&nbsp;
- **EINVAL**
The argument *path* is not a valid value.
&nbsp;
- **EBADF**
The argument *fd* is not a valid file descriptor.
&nbsp;
- **EROFS**
The *path* is on a read-only filesystem.
&nbsp;
- **EBUSY**
The device is busy.
&nbsp;
- **EMLINK**
Too many links exist of the path name *existing*.
&nbsp;
- **EACCES**
Do not permit to create a symbolic links.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
