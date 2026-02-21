# mq_getattr()
Retrieve attributes of a message queue

## Synopsis
```c
#include <mqueue.h>

int mq_getattr(mqd_t mqd, struct mq_attr *attr)
```

## Arguments

- **mqd**
The message queue descriptor.
&nbsp;
- **attr**
The attributes of the message queue.

## Library
libc

## Description
mq_getattr() retrieves attributes of the message queue referred to by the message queue descriptor *mqd*.
&nbsp;
mq_getattr() returns an mq_attr structure in the buffer pointed to by *attr*.
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

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO
