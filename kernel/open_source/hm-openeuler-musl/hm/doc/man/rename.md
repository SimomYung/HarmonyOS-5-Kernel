# rename(), renameat()
Change the name or path of a file

## Synopsis
```c
#include <stdio.h>

int rename(const char *old, const char *new);

#include <fcntl.h>
#include <stdio.h>

int renameat(int oldfd, const char *old, int newfd, const char *new);
```

## Arguments

- **old**
The old path of the specified file.
&nbsp;
- **new**
The new path of the specified file.
&nbsp;
- **oldfd**
The file descriptor of the path of the specified file.
&nbsp;
- **newfd**
The file descriptor of the new path of the specified file.

## Library
libc

## Description

rename() changes the name and the path of a file, moving it to specified directory if required. If the new path already exists, it will be atomically replaced. If the old path and new path are existing hard links referring to the same file, then rename() does nothing, and returns a success status. If the old path is a directory, then the new path must either not exist, or it must be an empty directory. If the old path refers to a symbolic link, the symbolic link is renamed.
&nbsp;
renameat() operates in the same way as rename(), except for some differences described here. If the old path is relative, then it is interpreted relative to the directory referred to by the file descriptor *oldfd*. If old path is relative and *oldfd* equals the special value AT_FDCWD, then the old path is interpreted relative to the current working directory of the calling process. If the old path is absolute, then *oldfd* is ignored. The interpretation of new path is same as for old path, except that a relative pathname is interpreted relative to the directory referred to by the file descriptor *newfd*.

## Returns
On success, these functions return 0. On error, -1 is returned, and errno is set to indicate the error.
&nbsp;
## Errors
- **EFAULT**
The *old* or *new* is not valid.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EBADF**
The argument *oldfd* or *newfd* is not valid.
&nbsp;
- **ENAMETOOLONG**
The argument *old* or *new* is too long.
&nbsp;
- **EROFS**
Readonly filesystem.
&nbsp;
- **EXDEV**
The mount points are different.
&nbsp;
- **EISDIR**
The argument *old* is a file but the *new* is a directory.
&nbsp;
- **ENOTDIR**
The argument *old* is a directory but the argument *new* is not a directory.
&nbsp;
- **ENOTDIR**
"old" is relative and *oldfd* is a file descriptor referring to a file other than a directory;or similar for *new* and *newfd*
&nbsp;
- **EOPNOTSUPP**
The operation is not supported.
&nbsp;
- **ENOENT**
The argument *old* does not exist.
&nbsp;
- **EBUSY**
The *old* or *new* is used as working directoy or root directoy.
&nbsp;
- **EINVAL**
The *new* contained a path prefix of the *old* or was made to make a directory a subdirectory of itself.
&nbsp;
- **ELOOP**
Too many symbolic links are encountered in resolving *old* or *new*
&nbsp;
- **ENOTEMPTY/EEXIST**
"new" is a nonempty directory, that is, contains entries other than "." and "..".

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
