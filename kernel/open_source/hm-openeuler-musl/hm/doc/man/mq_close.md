# mq_close()
Close a message queue descriptor

## Synopsis
```c
#include <mqueue.h>

int mq_close(mqd_t mqd);
```

## Arguments

- **mqd**
The message queue descriptor.

## Library
libc

## Description
mq_close() closes the message queue descriptor *mqd*.
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
