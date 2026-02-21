# umount2()
Unmount a filesystem (or directory)

## Synopsis
```c
#include <sys/mount.h>

int umount2(const char *special, int flags);
```

## Arguments

- **special**
The target directory need to be unmounted.
&nbsp;
- **flags**
The flags for unmount setting.

## Library
libc

## Description
The umount2() function unmounts the specified directory with the flags set up. Currently, the flags is supported as flow:
UMOUNT\_NOFOLLOW and MNT\_DETACH supported. MNT\_FORCE ignored. MNT\_EXPIRE not supported, return EINVAL.

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

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
