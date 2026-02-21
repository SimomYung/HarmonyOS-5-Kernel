# utimensat()
Change file timestamps with nanosecond precision

## Synopsis
```c
#include <fcntl.h>
#include <sys/stat.h>

int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags);
```

## Arguments
- **dirfd**
A file descriptor referring to the parent directory.
&nbsp;
- **pathname**
The argument *pathname* is a relative name of a file in the directory.
&nbsp;
- **times[2]**
New file timestamps specified in the array times. times[0] specifies the new "last access time" (atime), and times[1] specifies the new "last modification time" (mtime).
&nbsp;
- **flags**
The argument *flags* is a bit mask which can be *0* or *AT_SYMLINK_NOFOLLOW*. When *flags* is *AT_SYMLINK_NOFOLLOW* and *pathname* specifies a symbolic link the timestamps of the link file will be updated other than the file it refers.

## Library
libc

## Description
The function utimensat() updates the timestamps of file pointed to by *dirfd* *pathname* with nanosecond precision.

## Returns
- **0**
Update the timestamps of the file successfully.
&nbsp;
- **-1**
An error occurs, -1 is returned and the *errno* is set.

## Errors
- **EACCES**
The argument *times* is NULL or both *tv_nsec* values are *UTIME_NOW* and either the effective user does not have the write access to the file or the file is marked immutable.
&nbsp;
- **EBADF**
The argument *pathname* is a relative pathname , but *dirfd* is neither *AT_FDCWD* nor a valid file descriptor.
&nbsp;
- **EFAULT**
The argument *times* pointed to an invalid address or *dirfd* was AT_FDCWD and pathname is NULL or an invalid address.
&nbsp;
- **EINVAL**
*flags*, *tv_nsec* and *tv_sec* fields are invalid values. In addition, *pathname* is NULL, *dirfd* is not *AT_FDCWD* or *flags* contains *AT_SYMLINK_NOFOLLOW*.
&nbsp;
- **ELOOP**
Too many symbolic links are encountered in resolving *pathname*.
&nbsp;
- **ENAMETOOLONG**
The length of *pathname* is greater than *{MAX_PATH}*.
&nbsp;
- **ENOENT**
The argument *pathname* is an empty string or it does not refer to an existing file or directory.
&nbsp;
- **ENOTDIR**
The argument *pathname* is a relative pathname, but *dirfd* is neither a file descriptor referring to a directory nor *AT_FDCWD*.
&nbsp;
- **EPERM**
The caller tried to change one or both timestamps to some values other than the current time, or to change only one of the timestamps to the current time and either the effective user is not the owner of the file, or the file is marked append-only or immutable.
&nbsp;
- **EROFS**
The file is on a read-only filesystem.
&nbsp;
- **ESRCH**
The prefix components of *pathname* is denied for searching.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO



