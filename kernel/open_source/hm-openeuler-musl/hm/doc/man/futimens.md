# futimens()
Set the access and modification times of a file

## Synopsis
```c
#include <fcntl.h>
#include <sys/stat.h>

int futimens(int fd, const struct timespec times[2]);
```

## Arguments
- **fd**
The file descriptor refers to the target file.
&nbsp;
- **times[2]**
The argument *times[2]* is an array of two *timespec* structures, the first array member represents the data and time of last access, and the second member represents the date and time of last modification.

## Library
libc

## Description
The futimens() function sets the access and modification times of a file pointed to by the file descriptor *fd* to the values of the argument *times[2]*.

## Returns
- **0**
Set the access and modification times of a file successfully.
&nbsp;
- **-1**
When an error occurs, -1 is returned and the times of file shall not be affected.

## Errors
- **EACCES**
The argument *times[2]* is a NULL pointer or *tv_nsec* fields of times are UTIME\_NOW and the user does not have the right to write the file times.
&nbsp;
- **EINVAL**
Either of the argument *times[2]* specified a *tv_nsec* value as neither UTIME\_NOW nor UTIME\_OMIT, and is a value less than 0 or greater than 1000 million that is not supported by the file system.
&nbsp;
- **EPERM**
The argument *times[2]* is effective and the user does not have the privileges.
&nbsp;
- **EROFS**
The file is read-only in the file system.
&nbsp;
- **EBADF**
The argument *fd* is an invalid file descriptor.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
