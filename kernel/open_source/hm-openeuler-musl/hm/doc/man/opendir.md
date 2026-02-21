# opendir()
Open a directory

## Synopsis
```c
#include <dirent.h>
#include <fcntl.h>

DIR *opendir(const char *name);
```

## Arguments

- **name**
The name of the directory that will be opened.

## Library
libc

## Description
The opendir() function performs the open operation on a directory specified by the input parameter *name*.

## Returns

- **DIR**
The returned directory pointer by function (corresponding to the inputted dir name) when it works successfully.
&nbsp;
- **NULL**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.

## Errors

- **EINVAL**
The *name* is invalid.
&nbsp;
- **EFAULT**
The directory name is invalid.
&nbsp;
- **ENOMEM**
There is not enough memory when perform the dir or path allocating.
&nbsp;
- **ENAMETOOLONG**
The directory name is too long.
&nbsp;
- **EOPNOTSUPP**
The operation is not supported yet.
&nbsp;
- **ENOENT**
Failed to find the dentry.
&nbsp;
- **EMFILE**
The process limit on the number of open file descriptors has been reached.
&nbsp;
- **EACCES**
Permission denied.
&nbsp;
- **ENOTDIR**
*name* is not a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
