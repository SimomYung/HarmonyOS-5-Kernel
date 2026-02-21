# truncate(), truncate64()
Shrink or extend the size of a file to the specified size

## Synopsis
```c
#include <unistd.h>

int truncate(const char *path, off_t length);
int truncate64(const char *path, off_t length);
```
## Arguments

- **path**
The file name to truncate.
&nbsp;
- **length**
The specified size to truncate the file.

## Library
libc

## Description
truncate() shrinks or extends the size of a file to the specified size *length*. If the file size is larger than the specified size, the extra data is lost. If the file is shorter, it is extended and the extended part is fill with zero.

Regards to 64 version, they follow:
```c
#define ftruncate64 ftruncate
```

## Returns
On success, the truncate() function returns 0. on error, the truncate() function returns -1, errno is set to indicate the error.
&nbsp;
## Errors
- **EINVAL**
The argument *length* is less than 0.
&nbsp;
- **ENAMETOOLONG**
The argument *path* is too long.
&nbsp;
- **ENOENT**
The argument *path* is a null string.
&nbsp;
- **EFAULT**
The address of argument *path* is not valid.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EMFILE**
The per-process limit on the number of open file descriptors has been reached.
&nbsp;
- **EROFS**
Read-only filesystem.
&nbsp;
- **ETXTBSY**
Can not access the file.
&nbsp;
- **EACCES**
No permission to access the file.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
