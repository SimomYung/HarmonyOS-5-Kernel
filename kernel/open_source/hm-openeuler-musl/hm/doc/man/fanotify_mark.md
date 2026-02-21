# fanotify\_mark()
Add, remove, or modify an fanotify mark on a filesystem object

## Synopsis
```c
#include <sys/fanotify.h>

int fanotify_mark(int fanotify_fd, unsigned int flags,
		  uint64_t mask, int dirfd, const char *pathname);
```

## Arguments

- **fanotify_fd**
Fanotify fd return by fanotify_init().
&nbsp;
- **flags**
Flags is a bit mask describing the modification to perform: FAN_MARK_ADD, FAN_MARK_REMOVE, FAN_MARK_FLUSH.
Flags control the action type of notifications: FAN_MARK_DONT_FOLLOW, FAN_MARK_ONLYDIR, FAN_MARK_MOUNT, FAN_MARK_IGNORED_MASK, FAN_MARK_IGNORED_SURV_MODIFY.
- FAN_MARK_ADD
The events in mask will be added to the mark mask (or to the ignore mask). mask must be nonempty or the error EINVAL will occur.
- FAN_MARK_REMOVE
The events in argument mask will be removed from the mark mask (or from the  ignore  mask).
mask must be nonempty or the error EINVAL will occur.
- FAN_MARK_FLUSH
Remove  either all mount or all non-mount marks from the fanotify group.
&nbsp;
- **mask**
Mask controls the event mask to watch: FAN_ACCESS, FAN_MODIFY, FAN_CLOSE_WRITE, FAN_CLOSE_NOWRITE, FAN_OPEN, FAN_CLOSE, FAN_ONDIR.
- FAN_ACCESS
Create an event when a file or directory (but see BUGS) is accessed (read).
- FAN_MODIFY
Create an event when a file is modified (write).
- FAN_CLOSE_WRITE
Create an event when a writable file is closed.
- FAN_CLOSE_NOWRITE
Create an event when a read-only file or directory is closed.
- FAN_OPEN
Create an event when a file or directory is opened.
- FAN_CLOSE
A file is closed (FAN_CLOSE_WRITE|FAN_CLOSE_NOWRITE).
- FAN_ONDIR
Create events for directories.
&nbsp;
- **dirfd**
Use dirfd and pathname to find a inode.
&nbsp;
- **pathname**
Use dirfd and pathname to find a inode.

## Library
libc

## Description
fanotify\_mark() adds a new watch, remove a existed watch or modifies an existing watch, for the file whose location is specified in pathname, superblock or mountpoint; the caller must have read permission for this file. The fanotify_fd argument is a file descriptor referring to the fanotify instance whose watch list is to be modified. The events to be monitored for pathname are specified in the mask bit-mask argument(permission check has not been supported).

## Returns

On success, fanotify_mark() return 0.
&nbsp;

On failure, -1 is returned.

## Errors

- **EBADF**
The *fd* is not an valid fanotify fd.
&nbsp;
- **EINVAL**
The event mask is not valid or was opened with FAN_CLASS_NOTIF and mask contains permission events.
&nbsp;
- **ENOENT**
Dirfd or Path is an invalid path.
&nbsp;
- **ENOMEM**
Mem alloc failed during operation.
&nbsp;
- **ENOSPC**
Total watch number exceeds max value allowed.
&nbsp;
- **ENOTDIR**
Flags contains FAN_MARK_ONLYDIR, and dirfd and pathname do not specify a directory.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
