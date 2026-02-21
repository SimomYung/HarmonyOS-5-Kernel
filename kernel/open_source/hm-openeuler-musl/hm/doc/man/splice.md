# splice()
Splice data from/to a pipe

## Synopsis
```c
#define _GNU_SOURCE
#include <fcntl.h>

ssize_t splice(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags);
```

## Arguments
- **fd_in**
The file descriptor refers to the source file.
&nbsp;
- **off_in**
The offset of source file.
&nbsp;
- **fd_out**
The file descriptor refers to the destination file.
&nbsp;
- **off_out**
The offset of destination file.
&nbsp;
- **len**
The size of data need to be spliced.
&nbsp;
- **flags**
The flag is used to define different behaviors when the filesystem performs splicing. Behaviors include:
&nbsp;
  - **SPLICE_F_NONBLOCK**
Do not block on I/O. This makes the splice pipe operations nonblocking, but splice() may still be blocked due to the file descriptors spliced from/to may be blocked, unless it has *O_NONBLOCK* flag.
&nbsp;
  - **SPLICE_F_MOVE**
Attempt to move data instead of copying data from/to a pipe. This is only a hint to the kernel, data is still copied if the kernel can not move data. In fact, the flag is a no-op but it is still permitted in a splice() call. In the future, a proper implementation may be restored.
&nbsp;

## Library
libc

## Description
The splice() function moves data between two file descriptors without copying between kernel address space and user address space. It transfers up to *len* bytes of data from the file descriptor *fd_in* to the file descriptor *fd_out*. One of the file descriptors (*fd_in*, *fd_out*) must refers to a pipe.
&nbsp;
File descriptors of socket files are not supported yet.

## Returns
On success, the number of bytes spliced from/to the pipe is returned. On error, -1 is returned and errno is set.

## Errors
- **EAGAIN**
*SPLICE_F_NONBLOCK* is specified in flags, and the operation will block.
&nbsp;
- **EBADF**
One or both file descriptors are invalid, or do not have proper read/write mode.
&nbsp;
- **EINVAL**
The target file is opened in append mode.
&nbsp;
Neither of the file descriptors refers to a pipe.
&nbsp;
The file descriptors *fd_in*, *fd_out* refer to the same pipe.
&nbsp;
- **ENOMEM**
Out of memory.
&nbsp;
- **ESPIPE**
Either *off_in* or *off_out* is not NULL, but the corresponding file descriptor refers to a pipe.
&nbsp;
- **ENOSYS**
The file system does not support splicing.
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
