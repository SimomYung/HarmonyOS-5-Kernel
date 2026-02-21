# msgctl()
System V message control operations

## Synopsis
```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

## Arguments

- **msqid**
The message queue descriptor.
&nbsp;
- **cmd**
Specify the control operations.
&nbsp;
- **buf**
Buffer used to store information of the message queue.

## Library
libc

## Description
The msgctl() function performs the control operation specified by *cmd* on the System V message queue with identifier *msqid*.

## Returns

On success, IPC_STAT, IPC_SET, and IPC_RMID return 0.  A successful IPC_INFO or MSG_INFO operation returns the index of the highest used entry in the kernel's internal array recording information about all message queues.  (This information can be used with repeated MSG_STAT or MSG_STAT_ANY operations to obtain information about all queues on the system.)  A successful MSG_STAT or MSG_STAT_ANY operation returns the identifier of the queue whose index was given in *msqid*.
&nbsp;
On error, -1 is returned with *errno* indicating the error.

## Errors

- **EACCES**
The argument *cmd* is equal to IPC_STAT or MSG_STAT, but the calling process does not have read permission on the message queue.
&nbsp;
- **EFAULT**
The argument *cmd* has the value IPC_SET or IPC_STAT, but the address pointed to by *buf* isn't accessible.
&nbsp;
- **EIDRM**
The message queue identifier *msqid* is removed from the system.
&nbsp;
- **EINVAL**
Invalid value for *cmd* or *msqid*.
&nbsp;
- **EPERM**
The argument *cmd* has the value IPC_SET or IPC_RMID, but the effective user ID of the calling process is not the creator (as found in msg_perm.cuid) or the owner (as found in msg_perm.uid) of the message queue.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
