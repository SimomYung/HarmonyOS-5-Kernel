# fgetxattr()
Obtain the extended attribute of an object

## Synopsis
```c
#include <sys/types.h>
#include <sys/xattr.h>

ssize_t fgetxattr(int fd, const char *name, void *value, size_t size);
```

## Arguments
- **fd**
The fd (file descriptor) of the open file.
&nbsp;
- **name**
The name of the attribute.
&nbsp;
- **value**
The place to store the returned attribute.
&nbsp;
- **size**
A guessing size of the initial buffer, but the buffer can be increased if the return value is greater.

## Library
libc

## Description
The function fgetxattr() will obtain the extended attribute of an object which is specified by the inputted parameters (i.e., *fd* and *name*).

## Returns

- **Positive number**
The real size of the extended attribute. And the value of the attribute is stored in *value*.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.

## Errors

- **EBADF**
*fd* is invalid.
&nbsp;
- **EFAULT**
*name* or *value* is invalid addr.
&nbsp;
- **E2BIG**
The size of the attribute value is large than the maximum size allowed.
&nbsp;
- **ENODATA**
The attribute does not exist.
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
