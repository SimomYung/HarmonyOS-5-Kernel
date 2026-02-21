# flistxattr()
Obtain the attribute name list of an object

## Synopsis
```c
#include <sys/types.h>
#include <sys/xattr.h>

ssize_t flistxattr(int fd, char *list, size_t size);
```

## Arguments
- **fd**
The fd (file descriptor) of the file (target file).
&nbsp;
- **list**
The place to store the returned attribute name list.
&nbsp;
- **size**
A guessing size of the initial buffer, but the buffer can be increased if the return value is greater.

## Library
libc

## Description
The function flistxattr() will obtain the attribute name list of the specified file (specified by the fd), and return it. More specifically, the length of list is the return value of function, and the name list will be stored in the parameter *list*.

## Returns

- **Positive number**
The real size of the obtained attribute name list.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.

## Errors

- **EBADF**
*fd* is invalid.
- **EFAULT**
*list* is invalid addr.
&nbsp;
- **E2BIG**
The size of the attribute value is large than the maximum size allowed.
&nbsp;
- **EOPNOTSUPP**
The attributes might not be supported yet.
&nbsp;
- **ERANGE**
The size of the value buffer is too small.
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
