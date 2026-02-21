# lseek64()
Reposition 64-bit read or write file offset

## Synopsis
```c
#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <unistd.h>

off64_t lseek64(int fd, off64_t offset, int whence);
```

## Arguments
- **fd**
The file descriptor points to the file.
&nbsp;
- **offset**
The argument *offset* refers to the offset which shall be sought.
&nbsp;
- **whence**
The location of the file offset.

## Library
libc

## Description
The lseek64() function repositions the file offset at a open file description pointed to by *fd* to the *offset* according to the directive *whence*.
&nbsp;
Supported whences:
&nbsp;
- SEEK\_CUR: The file offset is set to *offset* bytes.
&nbsp;
- SEEK\_SET: The file offset is set to its current location plus *offset* bytes.
&nbsp;
- SEEK\_END: The file offset is set to the size of file plus *offset* bytes.

## Returns
Reposition the file offset successfully, the offset location as measured in bytes from the beginning of the file is returned.

When an error occurs, (off_t)-1 is returned and *errno* is set.

## Errors
- **EBADF**
The argument *fd* is an invalid file descriptor.
&nbsp;
- **EINVAL**
The argument *whence* is invalid, or the resulting offset will be negative and beyond the end of a seekable device.
&nbsp;
- **EOVERFLOW**
The resulting file offset cannot be represented in off_t.
&nbsp;
- **EOVERFLOW**
The file descriptor *fd* points to a pipe, socket or FIFO.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
