# utimes()
Set time to a file

## Synopsis
```c
#include <sys/time.h>

int utimes(const char *path, const struct timeval times[2]);
```

## Arguments

- **path**
Path of the file whose time needs to be set.
&nbsp;
- **times[2]**
The *times[2]* is the timeval structure data where the last access time and modification time are stored.

## Library
libc

## Description
Function utimes() sets the last access time and modification time of the specific file (located by *path*) with the give *times[2]*. It is worth noting that if *times* is NULL, then function will set the current time to file's last access and last modification.
## Returns

- **0**
Function finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.

## Errors

- **EINVAL**
The input *path* is invalid.
&nbsp;
- **ENAMETOOLONG**
*path* is too long.
&nbsp;
- **EBADF**
Failed to obtain the *fd* according to *path*.
&nbsp;
- **EROFS**
The file is read only.
&nbsp;
- **ENOMEM**
There is not enough memory during the function (unlikely to happen).

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
