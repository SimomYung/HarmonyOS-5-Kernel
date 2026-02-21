# fallocate()
Allocate space for a file

## Synopsis

```c
#define _GNU_SOURCE
#include <fcntl.h>

int fallocate(int fd, int mode, off_t ofs, off_t len);
```

## Arguments

- **fd**
A file descriptor refers to a file.
&nbsp;
- **mode**
The allocate mode to perform to the *fd*.
&nbsp;
- **ofs**
The start offset of the file to handle.
&nbsp;
- **len**
The bytes number of the space to allocate.

## Library
libc

## Description
The fallocate() function allows to allocate disk space for the file descriptor *fd* from *ofs* continuing for *len* bytes. Only supported in tmpfs, fatfs, and ext2 currently. The argument *mode* includes one of the following modes: 0, FALLOC\_FL\_KEEP\_SIZE, FALLOC\_FL\_PUNCH\_HOLE.
- **0**
The file size will be changed when the *ofs*+*len* is greater than the file size.
&nbsp;
- **FALLOC_FL_KEEP_SIZE**
The file size will remain unchanged.
&nbsp;
- **FALLOC_FL_PUNCH_HOLE**
Deallocate space in the byte range starting at *ofs* and continuing for *len* bytes.
This mode must be ored with FALLOC\_FL\_KEEP\_SIZE in argument *mode*, and is only supported in tmpfs currently.
&nbsp;

## Returns
- **0**
On success, return the zero.
&nbsp;
- **-1**
On error, return -1 and errno is set.

## Errors

- **EINVAL**
The argument *ofs* is less than 0, or *len* is less than or equal 0.
&nbsp;
- **EBADF**
The argument *fd* is not valid., or is not opened for writing.
&nbsp;
- **EFBIG**
The arguments *ofs*+*len* exceeds the maxsize of file.
&nbsp;
- **EISDIR**
The argument *fd* refers to a directory.
&nbsp;
- **ENODEV**
The argument *fd* does not refers to a regular file or a block device.
&nbsp;
- **ESPIPE**
The argument *fd* refers to fifo.
&nbsp;
- **EOPNOTSUPP**
Operation is not supported.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
