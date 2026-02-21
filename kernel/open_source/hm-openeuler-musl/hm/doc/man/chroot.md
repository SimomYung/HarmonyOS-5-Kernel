# chroot()
Change the root directory to the specified path

## Synopsis
```c
#include <unistd.h>

int chroot(const char *path);
```

## Arguments

- **path**
The new path that will be set as the root directory.


## Library
libc

## Description
The chroot() function will change the root directory to *path*.

## Returns

- **0**
The function completes successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors
- **ENOENT**
The *path* is invalid (either length is 0 or leng is too long), failed to obtain the entry.
&nbsp;
- **EBADF**
Can not find the corresponding parent fd in the process of walking *path*.
&nbsp;
- **EINVAL**
Wrong flags in the process of walking *path*.
&nbsp;
- **EPERM**
When caller does not have enough privilege.
&nbsp;
- **ENOMEM**
There is not enough memory for this function (unlikely to happen).
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
