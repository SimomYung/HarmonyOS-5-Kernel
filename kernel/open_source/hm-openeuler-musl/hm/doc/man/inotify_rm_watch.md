# inotify\_rm\_watch()
Remove inotify watch

## Synopsis
```c
#include <sys/inotify.h>

int inotify_rm_watch(int fd, int wd);
```

## Arguments 

- **fd**
Inotify fd returned by inotify_init() or inotify_init1().
&nbsp;
- **wd**
Watch descriptor returned by inotify_add_watch().


## Library
libc

## Description
inotify\_rm\_watch() removes the watch associated with the watch descriptor wd from the inotify instance associated with the file descriptor fd.

Removing a watch causes an IN_IGNORED event to be generated for this watch descriptor.


## Returns

- **0**
Remove success.
&nbsp;

- **-1**
Remove failed.

## Errors

- **EBADF**
*fd* is not a valid file descriptor.
&nbsp;
- **EINVAL**
*wd* is not a valid watch descriptor.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
