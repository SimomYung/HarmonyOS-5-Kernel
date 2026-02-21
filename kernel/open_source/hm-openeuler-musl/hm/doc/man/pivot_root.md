# pivot_root()
Change the root mount of a mount namespace.

## Synopsis
```c
#include <sys/mount.h>

int pivot_root(const char *new_root, const char *put_old);
```
<u>Note</u>: Musl provides the wrapper for **pivot_root()**, while glibc does not;

## Arguments
- **new_root**
New root of the mount namespace.
&nbsp;
- **put_old**
The directory to put mountpoints of the old root.

## Library
Musl provides the wrapper for **pivot_root()**. Glibc does not have the corresponding wrapper; call it using syscall(2).

## Description
**pivot_root()** changes the root mount in the mount namespace of the calling process. More precisely, it moves the root mount to the directory *put_old* and makes *new_root* the new root mount. The calling process must have the **CAP_SYS_ADMIN** capability in the user namespace that owns the caller's mount namespace.

**pivot_root()** changes the root directory and the current working directory of each process or thread in the same mount namespace to *new_root* if they point to the old root directory. (See also NOTES.) On the other hand, **pivot_root()** does not change the caller's current working directory (unless it is on the old root directory), and thus it should be followed by a chdir("/") call.

The following restrictions apply:
- *new_root* and *put_old* must be directories.
- *new_root* and *put_old* must not be on the same mount as the current root.
- *put_old* must be at or underneath *new_root*; that is, adding some nonnegative number of "/.." prefixes to the pathname pointed to by *put_old* must yield the same directory as *new_root*.
- *new_root* must be a path to a mount point, but can't be "/". A path that is not already a mount point can be converted into one by bind mounting the path onto itself.
- The propagation type of the parent mount of *new_root* and the parent mount of the current root directory must not be MS_SHARED; similarly, if *put_old* is an existing mount point, its propagation type must not be MS_SHARED. These restrictions ensure that **pivot_root()** never propagates any changes to another mount namespace.
- The current root directory must be a mount point.

## Returns
- **0**
 function finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.

## Errors
- **EBUSY**
<u>new_root</u> or <u>put_old</u> is on the current root mount. (This error covers the pathological case where <u>new_root</u> is "/".)
- **EINVAL**
<u>new_root</u> is not a mount point.
&nbsp;
- **EINVAL**
<u>put_old</u> is not at or underneath <u>new_root</u>.
&nbsp;
- **EINVAL**
The current root directory is not a mount point (because of an earlier chroot(2)).
&nbsp;
- **EINVAL**
The current root is on the rootfs (initial ramfs) mount; see NOTES.
&nbsp;
- **EINVAL**
Either the mount point at <u>new_root</u>, or the parent mount of that mount point, has propagation type MS_SHARED.
&nbsp;
- **EINVAL**
<u>put_old</u> is a mount point and has the propagation type MS_SHARED.
&nbsp;
- **ENOTDIR**
<u>new_root</u> or <u>put_old</u> is not a directory.
&nbsp;
- **EPERM**
The calling process does not have the **CAP_SYS_ADMIN** capability.
&nbsp;

## Classification
**pivot_root()** is HongMeng and Linux specific, hence it is not portable.

## Notes
**pivot_root()** allows the caller to switch to a new root filesystem while at the same time placing the old root mount at a location under *new_root* from where it can subsequently be unmounted. (The fact that it moves all processes that have a root directory or current working directory on the old root directory to the new root frees the old root directory of users, allowing the old root mount to be unmounted more easily.)

One use of **pivot_root()** is during system startup, when the system mounts a temporary root filesystem (e.g., an initrd(4)), then mounts the real root filesystem, and eventually turns the latter into the root directory of all relevant processes and threads. A modern use is to set up a root filesystem during the creation of a container.

The fact that **pivot_root()** modifies process root and current working directories in the manner noted in DESCRIPTION is necessary in order to prevent kernel threads from keeping the old root mount busy with their root and current working directories, even if they never access the filesystem in any way.

The rootfs (initial ramfs) cannot be pivot_root()ed. The recommended method of changing the root filesystem in this case is to delete everything in rootfs, overmount rootfs with the new root, attach stdin/stdout/stderr to the new /dev/console, and exec the new init(1).
