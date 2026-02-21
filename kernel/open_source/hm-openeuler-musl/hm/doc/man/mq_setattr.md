# mq_setattr()
Modify attributes of a message queue

## Synopsis
```c
#include <mqueue.h>

int mq_setattr(mqd_t mqd, const struct mq_attr *restrict new, struct mq_attr *restrict old);
```

## Arguments

- **mqd**
The message queue descriptor.
&nbsp;
- **new**
The attributes of the message queue to be modified.
&nbsp;
- **old**
The struct to record attributes of the message queue.

## Library
libc

## Description
mq_setattr() modifies attributes of the message queue referred to by the message queue descriptor *mqd*.
&nbsp;
mq_setattr() sets message queue attributes using information supplied in the mq_attr structure pointed to by *new*. The only attribute that can be modified is the setting of the O_NONBLOCK flag in mq_flags. The other fields in *new* are ignored. If the *old* field is not NULL, then the buffer that it points to is used to return an mq_attr structure that contains the same information that is returned by mq_getattr().
&nbsp;
Message queue descriptors will not be inherited by the child process.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EBADF**
The message queue descriptor specified in *mqd* is invalid.
&nbsp;
- **EINVAL**
*new->mq_flags* contained set bits other than O_NONBLOCK.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO
