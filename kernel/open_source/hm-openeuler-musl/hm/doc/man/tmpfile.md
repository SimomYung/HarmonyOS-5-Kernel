# tmpfile(), tmpfile64()
Create a temporary file

## Synopsis
```c
#include <stdio.h>

FILE *tmpfile(void);
FILE *tmpfile64(void);
```
## Arguments
N/A

## Library
libc

## Description
The tmpfile() function creates and opens a unique temporary file. The file will be automatically deleted when it is closed or the calling process terminates.

Regards to 64 version, it follows:
```c
#define tmpfile64 tmpfile
```
## Returns
On success, the tmpfile() function returns a stream descriptor. On error, the tmpfile() function returns NULL if a unique filename cannot be generated or the unique file cannot be opened, errno is set to indicate the error.
## Errors
- **EBADF**
Can not get a valid file descriptor for the temporary file.
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
- **EACCES**
No permission to access the file.
&nbsp;
- **EPERM**
The users has no permission to access the file.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
