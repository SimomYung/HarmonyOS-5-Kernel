# removexattr()
Remove the extended attribute of an object

## Synopsis
```c
#include <sys/types.h>
#include <sys/xattr.h>

int removexattr(const char *path, const char *name);
```

## Arguments
- **path**
The path of the file (target file).
&nbsp;
- **name**
The name of the attribute.

## Library
libc

## Description
The function removexattr() will remove the extended attributes of file (located by the parameter *path* and *name*). It is worth pointing out that the extended attribute(s) include: name, or other information about the node, such as files, directories, symbolic links.

## Returns

- **0**
The function is finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.

## Errors

- **EFAULT**
*path* or *name* is invalid addr.
&nbsp;
- **ENODATA**
The attribute does not exist.
&nbsp;
- **EOPNOTSUPP**
The attributes might not be supported yet.
&nbsp;

## Example

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
