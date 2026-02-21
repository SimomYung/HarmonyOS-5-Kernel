# mkfifoat()
Make a spacial file -- FIFO (a named pipe)

## Synopsis
```c
#include <fcntl.h>
#include <sys/stat.h>

int mkfifoat(int dirfd, const char *pathname, mode_t mode);
```

## Arguments
- **dirfd**
The file descriptor that is refers to the parent directory.
&nbsp;
- **pathname**
The argument *pathname* is a relative path of the FIFO file.
&nbsp;
- **mode**
The argument *mode* specifies the permissions of the FIFO file.

## Library
libc

## Description
The function mkfifoat() makes a FIFO with file named *path* in */dev* directory, and the argument *mode* specifies the FIFO's permissions.

## Returns
- **0**
Make a FIFO successfully and 0 is returned.
&nbsp;
- **-1**
An error occurs, -1 is returned and *errno* is set appropriately.

## Errors
- **EACCES**
The component directory of *path* does not allow to search or execute.
&nbsp;
- **EEXIST**
The file pointed to by *path* already exists.
&nbsp;
- **ENAMETOOLONG**
The length of *path* is greater than *{PATH_MAX}*.
&nbsp;
- **ENOENT**
The directory component in *path* does not exist.
&nbsp;
- **ENOSPC**
The parent directory or the filesystem has no space for the new FIFO file.
&nbsp;
- **EROFS**
The argument *path* refers to a read-only filesystem.
&nbsp;
- **EBADF**
The argument *dird* is an invalid file descriptor.
&nbsp;
- **ENOTDIR**
The argument *dird* is a valid file descriptor and it refers to a file other than a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO



