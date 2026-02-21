# fremovexattr()
Remove the extended attribute of an object

## Synopsis
```c
#include <sys/types.h>
#include <sys/xattr.h>

int fremovexattr(int fd, const char *name);
```

## Arguments
- **fd**
The fd of the file (target file).
&nbsp;
- **name**
The name of the attribute.

## Library
libc

## Description
The function fremovexattr() will remove the extended attributes of file (located by the parameter *fd* and *name*). This function is similar to function removexattr(), the exception is in the case of the file belongs to symbolic link. In that case, function will remove the extended attributes of the link itself, not the origin file the link refers to.

## Returns

- **0**
The function is finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.

## Errors

- **EBADF**
*fd* is invalid.
- **EFAULT**
*name* is invalid addr.
&nbsp;
- **ENODATA**
The attribute does not exist.
&nbsp;
- **EOPNOTSUPP**
The attributes might not be supported yet.
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
