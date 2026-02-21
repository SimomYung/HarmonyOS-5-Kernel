# inotify\_add\_watch()
Add new watch on specified file to an inotify instance 

## Synopsis
```c
#include <sys/inotify.h>

int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
```

## Arguments 

- **fd**
Inotify fd return by inotify_init() or inotify_init1()
&nbsp;
- **pathname**
Pathname to watch on.
&nbsp;
- **mask**
Event mask to watch.


## Library
libc

## Description
inotify\_add\_watch() adds a new watch, or modifies an existing watch, for the file whose location is specified in pathname; the caller must have read permission for this file. The fd argument is a file descriptor referring to the inotify instance whose watch list is to be modified. The events to be monitored for pathname are specified in the mask bit-mask argument.

## Returns

On success, inotify_add_watch() return an wd great than or equal to zero which may be used later.
&nbsp;

On failure, -1 is returned.

## Errors

- **EACCES**
The caller has no read capability on path
&nbsp;
- **EBADF**
The *fd* is not an valid inotify fd.
&nbsp;
- **EINVAL**
The event mask is not valid.
&nbsp;
- **ENOENT**
Path is an invalid path.
&nbsp;
- **ENOMEM**
Mem alloc failed during operation.
&nbsp;
- **ENOSPC**
Total watch number exceeds max value allowed.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
