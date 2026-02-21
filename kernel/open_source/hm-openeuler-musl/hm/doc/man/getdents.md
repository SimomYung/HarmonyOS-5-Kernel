# getdents(), getdents64()
Obtain the dentries of a directory

## Synopsis
```c
#include <dirent.h>

int getdents(int fd, struct dirent *buf, size_t len);
int getdents64(int fd, struct dirent *buf, size_t len);
```

## Arguments

- **fd**
The file descriptor relates to the directory.
&nbsp;
- **buf**
The buffer directory to store the obtained dentry (or dentries).
&nbsp;
- **len**
The size of the buffer.

## Library
libc

## Description
The getdents() function, as well as getdents64(), obtains the dentry (or dentries) of a directory specified by *fd*. The return data will be stored in the buffer.

&nbsp;
Note that we use a fixed size(256B) array for d_name which may be different with others.

## Returns

- **Positive number**
The function will return the number of read bytes when it works successfully.
&nbsp;
- **0**
Function works, but didn't obtain anything.
&nbsp;
- **Negative number**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors
- **ENOTDIR**
The *fd* is not relating to any directory.
&nbsp;
- **EOPNOTSUPP**
Operations are not supported yet.
&nbsp;
- **ENOMEM**
There is not enough memory when perform the dir or dentry reading (unlikely to happen).
&nbsp;
- **ENOENT**
Error occurs when perform the dentry reading or formatting.
&nbsp;
- **EFAULT**
The dentry is NULL or the node information of dentry is NULL.
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
