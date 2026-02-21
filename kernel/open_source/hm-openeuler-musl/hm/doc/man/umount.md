# umount()
Unmount a filesystem (or directory)

## Synopsis
```c
#include <sys/mount.h>

int umount(const char *special);
```

## Arguments

- **special**
The target directory need to be unmounted.

## Library
libc

## Description
The umount() function unmounts the specified directory, it is similar to umount2() function. More details please refer to umount2().

## Returns

- **0**
The function finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors

- **EFAULT**
The input *special* is NULL.
&nbsp;
- **EINVAL**
The *special* is not NULL, but invalid.
&nbsp;
- **EBUSY**
The target directory is busy.
&nbsp;
- **ENOENT**
The target directory is not available.
&nbsp;
- **ENOMEM**
There is not enough memory during the function (unlikely to happen).
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
