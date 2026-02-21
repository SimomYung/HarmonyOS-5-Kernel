# mq_open()
Open or create a message queue.

## Synopsis
```c
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

mqd_t mq_open(const char *name, int flags);

mqd_t mq_open(const char *name, int flags, mode_t mode,
              struct mq_attr *attr);
```

## Arguments

- **name**
The name of the message queue to create or open.
&nbsp;
- **flags**
Flags that control the operation of the call.
&nbsp;
- **mode**
Specify the permissions to be placed on the new queue.
&nbsp;
- **attr**
Specify the maximum number of messages and the maximum size of messages that the queue will allow.

## Library
libc

## Description
mq_open() creates a new POSIX message queue or opens an existing queue. The queue is identified by *name*.
&nbsp;
The max length of *name* is 32.
&nbsp;
The flags argument specifies flags that control the operation of the call. Exactly one of the following must be specified in flags:
&nbsp;
- **O\_RDONLY**
Open the queue to receive message only.
&nbsp;
- **O\_WRONLY**
Open the queue to send message only.
&nbsp;
- **O\_RDWR**
Open the queue to both send and receive messages.
&nbsp;

Zero or more of the following flags can additionally be ORed in flags:
&nbsp;
- **O\_CREAT**
Create the message queue if it does not exist.
&nbsp;
- **O\_EXEC**
If O_CREAT was specified in flags, and a queue with the given name already exists, the fail with the error EEXIST.
&nbsp;
- **O\_CLOEXEC**
Set the close-on-exec flag for the message queue descriptor.
&nbsp;
- **O\_NONBLOCK**
Open the queue in nonblocking mode.
&nbsp;

Message queue doesn't share descriptors with filesystem, so the per-process limit on the number of open file and message queue descriptors (`RLIMIT_NOFILE`) doesn't apply, therefore the errno `EMFILE` won't be set in this function.
&nbsp;
Message queue descriptors will not be inherited by the child process.

## Returns

- **A message queue descriptor**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EACCES**
The queue exists, but the caller does not have permission to open it in the specified mode.
&nbsp;
- **EEXIST**
Both O_CREAT and O_EXCL were specified in *flags*, but a queue with this name already exists.
&nbsp;
- **EINVAL**
*name* doesn't follow the format.
&nbsp;
- **EINVAL**
O_CREAT was specified in *flags*, and *attr* was not NULL, but *attr->mq_maxmsg* or *attr->mq_msqsize* was invalid.  Both of these fields must be greater than zero. In a process that is unprivileged (does not have the **POSIX_CAP_SYS_RESOURCE** capability), *attr->mq_maxmsg* must be less than or equal to the *msg_max* limit, and *attr->mq_msgsize* must be less than or equal to the *msgsize_max* limit. In addition, even in a privileged process, *attr->mq_maxmsg* cannot exceed the HARD_MAX limit. Two */proc* files, */proc/sys/fs/mqueue/msg_max* and */proc/sys/fs/mqueue/msgsize_max*, can be used to control these limits. If *attr* is NULL, then the queue is created with implementation-defined default attributes. Two /proc files, */proc/sys/fs/mqueue/msg_default* and */proc/sys/fs/mqueue/msgsize_default*, can be used to control these limits.
&nbsp;
- **EMFILE**
The per-process limit on the number of message queue descriptors has been reached.
&nbsp;
- **ENAMETOOLONG**
*name* was too long.
&nbsp;
- **ENFILE**
The system-wide limit on the total number of message queues has been reached.
&nbsp;
- **ENOENT**
*name* was just "/" followed by no other characters.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **ENOSPC**
Insufficient space for the creation of a new message queue. This probably occurred because the system-wide mqueue number limit was encountered; privileged processes (**POSIX_CAP_SYS_RESOURCE**) can exceed the limit. The */proc* file, */proc/sys/fs/mqueue/queues_max*, can be used to control this limit.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO
