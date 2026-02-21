# link(), linkat()
 - Create a link for a file

## Synopsis

```c
#include <unistd.h>
int link(const char *existing, const char *new);

#include <fcntl.h>
#include <unistd.h>
int linkat(int fd1, const char *existing, int fd2, const char *new, int flag);
```

## Arguments

- **existing**
The path name of a existing directory.
&nbsp;
- **new**
The new path name of the link which will be created by link or linkat function.
&nbsp;
- **fd1**
A file descriptor refers to a directory which also can be the constant AT_FDCWD.
&nbsp;
- **fd2**
A file descriptor refers to a directory which also can be the constant AT_FDCWD.
&nbsp;
- **flag**
AT_SYMLINK_FOLLOW
By default, linkat() does not dereference oldpath if it is a symbolic link (like link()). The flag AT_SYMLINK_FOLLOW can be specified in flags to cause oldpath to be dereferenced if it is a symbolic link.
Flag AT_EMPTY_PATH is not supported and will return error. PS: Errno is ENOENT.

## Library
libc

## Description
link() creates a new link(also known as a hard link) named *existing* for a existing file. If the new path named *new* exists, it will run failed and will return -1.
&nbsp;
linkat() also creates a new link as link(), but the arguments have some differences.
If the path name given in *existing* or *new* is relative, then it is interpreted relative to the directory referred to by the file descriptor fd1 or fd2. If the path name is relative and fd equals constant AT_FDCWD, then the path name is interpreted relative to the current working directory of the calling process. If the path name is absolute, then the argument fd is ignored.

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
The argument *fd* is not a valid value or the length of *existing* or *new* is not enough, or An invalid flag value was specified in *flags*.
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
&nbsp;
- **EXDEV**
Invalid cross-device link.
&nbsp;
- **ELOOP**
Too many symbolic links are encountered in resolving *existing* or *new*.
&nbsp;
- **ENOSPC**
*new* was to be created but the device has no room for the new file.
&nbsp;
- **ENOTDIR**
"existing"/"new" is relative and "fd1"/"fd2" is a file descriptor referring to a file other than a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
