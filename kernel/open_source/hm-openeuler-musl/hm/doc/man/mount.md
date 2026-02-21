# mount()
Mount a filesystem to a directory

## Synopsis
```c
#include <sys/mount.h>

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data);
```

## Arguments

- **special**
The source file (or device) need to be mounted.
&nbsp;
- **dir**
The target directory.
&nbsp;
- **fstype**
The type of filesystem.
&nbsp;
- **flags**
The flgas for mount.
&nbsp;
- **data**
Mount options for the endpoint filesystem.

## Library
libc

## Description
Function mount() mounts one file (or device) to another directory, for the purpose of easily accessing the source file (or device). Now, we support basic mount function.
&nbsp;
Supported filesystems: resmgrfs, procfs, sysfs, devfs, tmpfs, vfat, ext2, jffs2, hugetlbfs, overlayfs, ubifs, squashfs, and ext4 which should be used with option argument "transfs".
&nbsp;
Supported mount flags: MS\_RDONLY, MS\_SYNCHRONOUS, MS\_DIRSYNC, MS\_REMOUNT, MS\_BIND, MS\_NOEXEC, MS\_NODEV, MR\_REC, MR\_MOVE.
&nbsp;
Note that not all filesystems support remount, only tmpfs, jffs2, ext2 and overlayfs support remount for now.
&nbsp;


## Returns

- **0**
Mount function finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors

- **EFAULT**
The input *special* or *dir* is NULL.
&nbsp;
- **EBADF**
Failed to obtain the *fd* of the input dir.
&nbsp;
- **EINVAL**
The *special* or *dir* is not NULL, but invalid.
&nbsp;
- **EBUSY**
The target directory is busy.
&nbsp;
- **ENOTDIR**
The *dir* is not directory.
&nbsp;
- **ENOMEM**
There is not enough memory during the function (unlikely to happen).

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## Note
procfs, devfs and sysfs can be mounted mulitple times, but the first mount point must not be umounted and return EBUSY error.
Creating directory in resmgrfs will cause failure of umounting.

Currently, Hongmeng processes mount propagation differently from that of Linux in the following case: if the mountpoint dentry in target namespace has already been mounted (i.e., overmount a slave mount entry and get propagation from master which is targeting on the same dentry), the propagation would fail with EBUSY.
