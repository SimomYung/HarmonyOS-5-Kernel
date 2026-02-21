# fanotify\_init()
Create and initialize fanotify group

## Synopsis
```c
#include <fcntl.h>
#include <sys/fanotify.h>

int fanotify_init(unsigned int flags, unsigned int event_f_flags);
```

## Arguments

- **flags**
Flags control the notification classes are FAN_CLASS_PRE_CONTENT, FAN_CLASS_CONTENT, FAN_CLASS_NOTIF
Only use FAN_CLASS_PRE_CONTENT/FAN_CLASS_CONTENT/FAN_CLASS_NOTIF to recevie events in order now.

Flags control the open are FAN_CLOEXEC, FAN_NONBLOCK.
- FAN_CLOEXEC
Set the close-on-exec flag (FD_CLOEXEC) on the new file descriptor.
- FAN_NONBLOCK
Enable the nonblocking flag (O_NONBLOCK) for the file descriptor.

Flags control notification queue/mark are FAN_UNLIMITED_QUEUE, FAN_UNLIMITED_MARKS.
- FAN_UNLIMITED_QUEUE
Remove the limit of 16384 events for the event queue. Use of this flag requires the CAP_SYS_ADMIN capability.
- FAN_UNLIMITED_MARKS
Remove the limit of 8192 marks. Use of this flag requires the CAP_SYS_ADMIN capability.
&nbsp;
- **event_f_flags**
Only support O_RDONLY now.
- O_RDONLY
This value allows only read access.
&nbsp;


## Library
libc

## Description
The fanotify_init() function create and initialize a fanotify group, and retrun a fanotify fd.

## Returns

On success, function return an fd great than or equal to zero.
&nbsp;

On failure, -1 is returned.

## Errors

- **EINVAL**
Invalidate flags or event_f_flags was given.
&nbsp;
- **EMFILE**
The number of fanotify groups for this user exceeds 128 or number of open files reach rlimit.
&nbsp;
- **ENOMEM**
Mem alloc failed during init.
&nbsp;
- **EPERM**
Operation is not permitted because the caller lacks the CAP_SYS_ADMIN capability.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
