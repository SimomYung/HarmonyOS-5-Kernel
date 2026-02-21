# fchmodat()
Change mode of a file relative to a directory

## Synopsis
```c
#include <sys/stat.h>
#include <fcntl.h>

int fchmodat(int dirfd, const char *pathname, mode_t mode, int flags);
```

## Arguments
- **dirfd**
The file descriptor refers to the parent directory.
&nbsp;
- **pathname**
A relative path of the file.
&nbsp;
- **mode**
The permission modes will set to the file.
&nbsp;
- **flags**
The value of *flags* is *0* or *AT_SYMLINK_NOFOLLOW*. When *AT_SYMLINK_NOFOLLOW* is set, if *pathname* refers to a symbolic link, the mode of the symbolic link is changed.

## Library
libc

## Description
The fchmodat() function changes user's permission modes of the file pointed by *dirfd* and *pathname* to *mode* argument. If the value of *flags* is *AT_SYMLINK_NOFOLLOW* and *pathname* refers to a symbolic link, the mode of symbolic link is changed.

## Returns
- **0**
Change the file permission modes successfully.
&nbsp;
- **-1**
When an error occurred, the file permission modes will not change. *-1* will be returned and *errno* is set.

## Errors
- **EACCES**
*dirfd* is not opened with *O_SEARCH* and the directory does not permit directory searches.
&nbsp;
- **EBADF**
*pathname* does not specify an absolute path and *dirfd* is neither a file descriptor open for searching or reading nor *AT_FDCWD*.
&nbsp;
- **ENOTDIR**
The argument *pathname* is not an absolute path and *dirfd* refers to a file other than a directory.
&nbsp;
- **EINVAL**
The value of *mode* is invalid.
&nbsp;
- **ELOOP**
Too many symbolic links are encountered during resolution of *pathname*.
&nbsp;
- **ENAMETOOLONG**
The length of *pathname*is greater than *{PATH_MAX}*.
&nbsp;
- **EINVAL**
The value of *flags* is invalid.
&nbsp;
- **EOPNOTSUPP**
The argument *flags* is set as *AT_SYMLINK_NOFOLLOW* and *pathname* refers to a symbolic link, but the filesystem does not support changing the mode of a symbolic link.
&nbsp;
- **EFAULT**
The *pathname* points to an outside address.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO 



