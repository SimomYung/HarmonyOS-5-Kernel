# copy_file_range()
Copy a range of data from one file to another

## Synopsis
```c
#define _GNU_SOURCE
#include <unistd.h>

ssize_t copy_file_range(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags);
```

## Arguments

- **fd_in**
The source file descriptor.
&nbsp;
- **off_in**
A pointer points the offset to the file descriptor *fd_in*.
&nbsp;
- **fd_out**
The target file descriptor.
&nbsp;
- **off_out**
A pointer points the offset to the file descriptor *fd_out*.
&nbsp;
- **len**
The bytes number to copy from *fd_in* to *fd_out*.
&nbsp;
- **flags**
This argument currently must be set to 0.

## Library
libc

## Description
The function copy_file_range() copies up to *len* bytes of data from the source file descriptor *fd_in* to the target file descriptor *fd_out*, overwriting the data existing within the requested range of the target file. If *off_in* is NULL, then bytes are read from *fd_in* from the current file offset. If *off_in* is not NULL, then bytes are read from the specified offset by *off_in*. The semantics apply for *off_in*, and similar statements apply to *off_out*. *fd_in* and *fd_out* can refer to the same file, but the source and target ranges are not allowed to overlap. By now, copy_file_range() is implemented internally by read() and write(). So it is not recommended to use this interface to boost performance.

## Returns
On success, the copy_file_range() function returns the number of bytes copied from another. On error, -1 is returned and errno is set to indicate the error.

## Errors

- **EINVAL**
The argument of this function is not valid; or the source and target ranges are overlap.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EBADF**
The argument *fd_in* or *fd_out* is not a valid file descriptor.
&nbsp;
- **EFBIG**
An attempt was made to write at a position past the maximum file offset the system supports.

## Notes
Due to the multi-server architecture in HongMeng OS, the input and output FDs are not guaranteed to be in the same virtual address space (VAS). In order to support checking both files' attributes in the call of copy_file_range(), implementing a server-side FD Table would be necessary, such that FSMGR would have the ability to retrieve the desired attributes and look into them in one VAS.

As a result, the current implementation does not fully conform to POSIX 1003.1-2017. Specifically, the functionality of below use cases is limited:
- The **O_APPEND** flag is set for the open file description (see open(2)) referred to by the file descriptor *fd_out*.
- Either *fd_in* or *fd_out* is not a regular file.
- The requested source or destination range is too large to represent in the specified data types.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
