# mkdir()
create a directory

## Synopsis
```c
#include <sys/stat.h>
#include <sys/types.h>

int mkdir(const char *pathname, mode_t mode);
```

## Arguments
- **pathname**
The argument *pathname* refers to the target file.
&nbsp;
- **mode**
The argument *mode* specifies the file permissions of the new directory.

## Library
libc

## Description
The function mkdir() function tries to create a directory named *pathname*

## Returns
- **0**
Create a directory named *pathname* successfully.
&nbsp;
- **-1**
An error occurs, -1 is returned and the *errno* is set appropriately.

## Errors
- **EACCES**
The current process does not have the write permission of the parent directory.
&nbsp;
- **EEXIST**
The file pointed to by *pathname* already exists.
&nbsp;
- **EFAULT**
The address space is inaccessible for the current user.
&nbsp;
- **EINVAL**
The final component of the directory pointed to by *pathname* is invalid.
&nbsp;
- **ELOOP**
Too many symbolic links are encountered in resolving the argument *pathname*.
&nbsp;
- **EMLINK**
The number of links to parent directory is larger than *{LINK_MAX}*.
&nbsp;
- **ENAMETOOLONG**
The argument *pathname* is too long.
&nbsp;
- **ENOENT**
The directory component of pathname does not exist.
&nbsp;
- **ENOMEM**
Insufficient memory is available.
&nbsp;
- **ENOSPC**
The device containing *pathname* has no room for the new directory.
&nbsp;
- **ENOTDIR**
The component of *pathname* is not a directory.
&nbsp;
- **EPERM**
The directory pointed to by *pathname* is not allowed to create in the filesystem.
&nbsp;
- **EROFS**
The directory pointed to by *pathname* on a read-only filesystem.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
