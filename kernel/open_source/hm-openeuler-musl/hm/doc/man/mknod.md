# mknod()
Create a special or ordinary file

## Synopsis
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int mknod(const char *pathname, mode_t mode, dev_t dev);
```

## Arguments
- **pathname**
The argument *pathname* refers to a filesystem node.
&nbsp;
- **mode**
The argument *mode* specifies both the file mode to use and the type of node to be created.
&nbsp;
- **dev**
If the file type is *S_IFCHR* or *S_IFBLK* the *dev* specifies the major and minor numbers of the newly created device file, otherwise it is ignored.

## Library
libc

## Description
The function mknod() creates a filesystem node (file, device, special file and named pipe) in *devfs* which is named *pathname* with *mode* and *dev* attributes.

## Returns
- **0**
Create a file successfully, 0 is returned.
&nbsp;
- **-1**
An error occurs, -1 is returned and *errno* is set.

## Errors
- **EACCES**
The component of *pathname* does not have the permission to write or search.
&nbsp;
- **EEXIST**
The file pointed to by *pathname* already exists.
&nbsp;
- **EFAULT**
The argument *pathname* refers to the inaccessible address space.
&nbsp;
- **ELOOP**
Too many symbolic links are encountered in resolving "pathname".
&nbsp;
- **ENAMETOOLONG**
The length of *pathname* is greater than *{PATH_MAX}*.
&nbsp;
- **ENOSPC**
The device has no space for the new node.
&nbsp;
- **ENOTDIR**
A component of *pathname* refers to a file other than a directory.
&nbsp;
- **EROFS**
The argument *pathname* refers to a file on a read-only filesystem.
&nbsp;
- **EINVAL**
*mode* requested creation of something other than a regular file,  device  special  file,  FIFO  or socket.
&nbsp;
- **ENOENT**
A directory component in *pathname* does not exist or is a dangling symbolic link.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO


