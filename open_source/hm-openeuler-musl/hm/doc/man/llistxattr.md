# llistxattr()
Obtain the attribute name list of an object

## Synopsis
```c
#include <sys/types.h>
#include <sys/xattr.h>

ssize_t llistxattr(const char *path, char *list, size_t size);
```

## Arguments
- **path**
The path of the file (target file).
&nbsp;
- **list**
The place to store the returned attribute name list.
&nbsp;
- **size**
A guessing size of the initial buffer, but the buffer can be increased if the return value is greater.

## Library
libc

## Description
The function llistxattr() is very similar to function listxattr(), the only exception is in the case of the target file is symbolic link. In this situation, the returned list refers to the attribute name of the symbolic link, not the origin file.

## Returns

- **Positive number**
The real size of the obtained attribute name list.

- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.

## Errors

- **EFAULT**
*path* or *list* is invalid addr.
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
