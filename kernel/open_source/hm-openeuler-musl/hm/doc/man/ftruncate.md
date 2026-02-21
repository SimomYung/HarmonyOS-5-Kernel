# ftruncate(), ftruncate64()
Shrink or extend the size of a file to the specified size

## Synopsis
```c
#include <unistd.h>

int ftruncate(int fd, off_t length);
int ftruncate64(int fd, off_t length);
```
## Arguments

- **fd**
The file description to truncate.
&nbsp;
- **length**
The specified size to truncate the file.

## Library
libc

## Description
ftruncate() shrinks or extends the size of a file to the specified size *length*. If the file size is larger than the specified size, the extra data is lost. If the file is shorter, it is extended and the extended part is fill with zero.

The file must be open for writing.

Regards to 64 version, they follow:
```c
#define ftruncate64 ftruncate
```

## Returns
On success, the ftruncate() function returns 0. on error, the ftruncate() function returns -1, errno is set to indicate the error.
&nbsp;
## Errors
- **EBADF**
fd is not a valid file descriptor.
&nbsp;
- **EINVAL**
fd is not open for writing.
&nbsp;
- **EINVAL**
fd is not support truncate.
&nbsp;
- **EINVAL**
The argument *length* is less than 0.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EROFS**
Read-only filesystem.
&nbsp;
- **ETXTBSY**
Can not access the file.
&nbsp;
- **EACCES**
No permission to access the file.
&nbsp;
- **EFBIG**
The argument *length* is larger than the maximum file size. (XSI).
&nbsp;

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
