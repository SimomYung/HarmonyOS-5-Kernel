# fstatat()
Get the status of a file

## Synopsis
```c
#include <fcntl.h>
#include <sys/stat.h>

int fstatat(int fd, const char *restrict path, struct stat *restrict buf, int flag);
```

## Arguments 
- **fd**
The file descriptor refers to the directory in which status of the target file shall be retrieved.
&nbsp;
- **path**
The argument *path* refers to the file in the directory which is pointed to by "fd".
&nbsp;
- **buf**
A pointer to a stat structure in which the status information of a file is stored.
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
The fstatat() function gets status information of a file and write it to the space pointed to by *buf*.

## Returns
- **0**
Get the file status information successfully.
&nbsp;
- **-1**
Failed to get the file status information.

## Errors
- **EACCES**
The directory pointed to by fd is not opened with O_SEARCH or permissions of it does not allow to search.
&nbsp;
- **EBADF**
The argument *path* does not specify an absolute path and the value of *fd* is neither AT_FDCWD nor a valid file descriptor for reading or searching.
&nbsp;
- **ENOTDIR**
The argument *path* is not an absolute one or *fd* is a file descriptor for a non-directory file.
&nbsp;
- **ELOOP**
Symbolic links of the file pointed to by *path* are more than *{SYMLOOP_MAX}*.
&nbsp;
- **ENAMETOOLONG**
The length of *path* is longer than *{PATH_MAX}*.
&nbsp;
- **EOVERFLOW**
A value of file status information to be stored overflows one of members in stat struct.
&nbsp;
- **EINVAL**
The value of *flag* is invalid.
&nbsp;
- **ENOENT**
A component of *path* does not exist or is a dangling symbolic link.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
