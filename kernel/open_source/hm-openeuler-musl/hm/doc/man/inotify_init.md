# inotify\_init(), inotify\_init1()
Create an inotify fd

## Synopsis
```c
#include <unistd.h>

int inotify_init();
int inotify_init1(int flags);
```

## Arguments 

- **flags**
Flags control fd create, possible bitmask values are IN_NONBLOCK, IN_CLOEXEC.
&nbsp;


## Library
libc

## Description
The inotify_init() function create an inotify fd, if flags is 0, inotify_init1(0) is equal with inotify_init().

## Returns

On success, function return an fd great than or equal to zero.
&nbsp;

On failure, -1 is returned.

## Errors

- **EINVAL**
Invalidate flags was given.
&nbsp;
- **ENOMEM**
Mem alloc failed during init.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
