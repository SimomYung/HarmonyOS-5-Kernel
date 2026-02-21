# fsetxattr()
Set the extended attribute to an object

## Synopsis
```c
#include <sys/types.h>
#include <sys/xattr.h>

int fsetxattr(int fd, const char *name, const void *value, size_t size, int flags);
```

## Arguments
- **fd**
The fd of the file (target file).
&nbsp;
- **name**
The name of the attribute.
&nbsp;
- **value**
The detail of attribute content (set to the file).
&nbsp;
- **size**
The specified size of the value content.
&nbsp;
- **flags**
The flag is used to refine the semantics of different operations of function setxattr(). Such as,
&nbsp;
    - **XATTR_CREATE**
        Create the attributes.
&nbsp;
    - **XATTR_REPLACE**
        Replace the old attributes.

## Library
libc

## Description
The function fsetxattr() sets extended attributes to a file (file is identified according to the parameter *fd* and *name*). It is very similar to setxattr(), the exception is in the case of file belongs to symbolic link. In that case, the attributes will be set on the link, not the origin file the link refers to.

## Returns

- **0**
The function is finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.

## Errors

- **EBADF**
*fd* is invalid.
&nbsp;
- **EFAULT**
*name* or *value* is invalid.
&nbsp;
- **EOPNOTSUPP**
The namespace prefix of name is not valid.
The attributes might not be supported yet.
&nbsp;
- **EEXIST**
**XATTR_CREATE** was specified, and the attribute exists already.
&nbsp;
- **ENODATA**
**XATTR_REPLACE** was specified, and the attribute does not exist.
- **ENOSPC**
There is insufficient space remaining to store the attribute.
- **ERANGE**
The size of name or value exceeds a filesystem-specific limit.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
