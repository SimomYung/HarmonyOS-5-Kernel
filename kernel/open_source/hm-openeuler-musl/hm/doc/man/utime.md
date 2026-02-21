# utime()
Set the modification time of a file

## Synopsis
```c
#include <sys/types.h>
#include <utime.h>
#include <time.h>

int utime(const char *path, const struct utimbuf *times);
```
## Arguments

- **path**
Path of the file whose time need to be set.
&nbsp;
- **times**
The time variable set by the caller. If *times* is NULL, then function will set the current time.

## Library
libc

## Description
Function utime() sets the last access time and modification time of the specific file (located by *path*).

## Returns

- **0**
Function finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

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
