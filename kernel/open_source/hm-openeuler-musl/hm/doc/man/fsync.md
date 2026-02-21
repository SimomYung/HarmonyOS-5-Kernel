# fsync(), fdatasync()
Synchronize a file cached writes to persistent storage

## Synopsis
```c
#include <unistd.h>

int fsync(int fd);
int fdatasync(int fd);
```
## Arguments

- **fd**
An open file descriptor to a file.

## Library
libc

## Description
fsync() flushes all modified information associated with the file *fd* into persistent storage.
fdatasync() is identical to fsync().

## Returns

- **0**
On success, return 0.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EBADF**
The argument *fd* is not valid.
&nbsp;
- **ENOMEM**
Insufficient memory was available.
&nbsp;
- **EINVAL**
The argument *fd* is not allowed to call sync.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
