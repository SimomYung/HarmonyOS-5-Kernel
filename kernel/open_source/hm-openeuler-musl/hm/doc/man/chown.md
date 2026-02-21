# chown(), fchown(), lchown(), fchownat()
Change file owner and group

## Synopsis
```c
#include <unisted.h>

int chown(const char *path, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int lchown(const char *path, uid_t owner, gid_t group);

#include <fcntl.h>
#include <unistd.h>

int fchownat(int dfd, const char *path, uid_t owner, gid_t group, int flag);
```

## Arguments

- **path**
The path to the file or directory that you want to change the ownership.
&nbsp;
- **owner**
The user id.
&nbsp;
- **group**
The group id.
&nbsp;
- **fd**
The open file descriptor *fd* that you want to change the ownership.
&nbsp;
- **flag**
By default *flag* is zero, and support two flags:
   - AT_EMPTY_PATH
     If *path* is an empty string, operate on the file referred to by *dfd*.
     In this case *dfd* can refer to any type of file, not just a directory.
     And AT_FDCWD is valid too.

   - AT_SYMLINK_NOFOLLOW
     If *path* is a symlink, do not dereference it.

## Library
libc

## Description
chown() changes the ownership of the file specified by *path*, which is dereferenced if it is a symbolic link.
&nbsp;
fchown() changes the ownership of the file referred to by the open file descriptor fd.

If fd is opened by O_PATH, syscall fchown will fail returning EBADF, but libc will try to use chown or fchownat to do chown by procfs and fd get the path of fd.
&nbsp;
lchown() changes the ownership of the file specified by *path*, but does not dereference symbolic links.
&nbsp;
fchownat() operates in the same way as chown(), except for some differences described here. If the *path* given is relative, then it is resolved relative to the directory referred to by the file descriptor fd. If *path* is relative and *fd* equals the special value AT_FDCWD, then *path* is resolved relative to the current working directory of the calling process. If *path* is absolute, then fd is ignored. The argument *flag* is a bit mask and is zero by default, and only AT_SYMLINK_NOFOLLOW is supported, If *path* is a symbolic link, operate on the link itself not to dereference it, like lchown().

## Returns

- **0**
On success, zero is returned.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EACCES**
No permissions.
&nbsp;
- **ENAMETOOLONG**
The path is too long.
&nbsp;
- **ENOENT**
The file does not exist.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EBADF**
The file descriptor is invalid.
&nbsp;
- **EROFS**
The file is on a read-only filesystem.
&nbsp;
- **EFAULT**
The *path* points to an outside address.
&nbsp;
- **ELOOP**
Too many symbolic links were encountered in resolving *path*.
&nbsp;
- **ENOTDIR**
The *path* is not a directory.
&nbsp;
- **EPERM**
The calling process did not have the requireed permissions.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
