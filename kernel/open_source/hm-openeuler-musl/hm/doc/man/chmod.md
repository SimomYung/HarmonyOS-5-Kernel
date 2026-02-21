# chmod()
Change permission modes of a file

## Synopsis
```c
#include <sys/stat.h>

int chmod(const char *pathname, mode_t mode);
```

## Arguments
- **pathname**
The path point to the file whose permission modes will be changed.
&nbsp;
- **mode**
The file's new permission modes.

## Library
libc

## Description
The chmod() function changes user's permission modes of the file pointed to by *pathname* to *mode*.

## Returns
- **0**
Change the file permission modes successfully.
&nbsp;
- **-1**
When an error occurred, the file permission modes will not change. *-1* will be returned and *errno* is set.

## Errors
- **EACCES**
Looking up the path of the file is denied.
&nbsp;
- **ELOOP**
The *pathname* is a symbolic link which contains a loop.
&nbsp;
- **ENAMETOOLONG**
The length of the *pathname* is longer than *NAME_MAX*.
&nbsp;
- **ENOENT**
The *pathname* is an empty string or the file it pointed to does not exist.
&nbsp;
- **ENOTDIR**
The prefix of *pathname* points to an existing file that is neither a directory nor a symbolic link to a directory.
&nbsp;
- **EPERM**
The effective user does not own the appropriate privileges.
&nbsp;
- **EROFS**
The file pointed to by *pathname* resides on a Read-Only file system.
&nbsp;
- **EFAULT**
The *pathname* points to an outside address.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
