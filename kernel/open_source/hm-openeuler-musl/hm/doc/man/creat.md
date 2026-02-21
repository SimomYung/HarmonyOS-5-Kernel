# creat()
Open or create a file or device.

## Synopsis
```c
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int creat(const char *filename, mode_t mode);
```

## Arguments

- **filename**
The name of the object file (either regular file or device).
&nbsp;
- **mode**
The mode set up for the operation types.

## Library
libc

## Description
The creat() function will invoke open(filename, O\_CREAT|O\_WRONLY|O\_TRUNC, mode) directly, so more details are shown in open().

## Returns

On success, creat() returns the new file descriptor of the file created. On error, -1 is returned and errno is set to indicate the error.

## Errors

- **EFAULT**
The address of the argument *filename* is invalid.
&nbsp;
- **ENOENT**
The length of the argument *filename* is zero.
&nbsp;
- **ENAMETOOLONG**
The argument *filename* is too long.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EROFS**
The argument *filename* refers to a file on a read-only filesystem and write access was requested.
&nbsp;
- **EMFILE**
The per-process limit on the number of open file descriptors has been reached.
&nbsp;
- **EACCES**
The requested access to the file is not allowed.
&nbsp;
- **EPERM**
The operation is not permitted.
&nbsp;
- **EEXIST**
The *filename* already exists and O_CREAT and O_EXCL were used.
&nbsp;
- **EISDIR**
The *filename* refers to an existing directory.
&nbsp;
- **ELOOP**
Too many symbolic links were encountered in resolving *filename*.
&nbsp;
- **ENOSPC**
*filename* is to be created but the device has no room for the new file.
&nbsp;
- **ETXTBSY**
*filename* refers to a file that is currently in use.
&nbsp;

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
