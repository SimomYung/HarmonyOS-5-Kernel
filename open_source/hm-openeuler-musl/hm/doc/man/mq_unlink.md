# mq_unlink()
Remove a message queue

## Synopsis
```c
#include <mqueue.h>

int mq_unlink(const char *name)
```

## Arguments

- **name**
The specified message queue name.

## Library
libc

## Description
mq_unlink() removes the specified message queue *name*. The message queue name is removed immediately. The queue itself is destroyed once any other processes that have the queue open close their descriptors referring to the queue.
&nbsp;
The max length of *name* is 32.
&nbsp;
Message queue descriptors will not be inherited by the child process.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*name* only contains '/'.
&nbsp;
- **ENAMETOOLONG**
*name* was too long.
&nbsp;
- **ENOENT**
There is no message queue with the given *name*.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO
