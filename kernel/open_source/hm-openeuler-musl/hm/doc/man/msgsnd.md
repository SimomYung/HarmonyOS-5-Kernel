# msgsnd()
XSI message send operation

## Synopsis
```c
#include <sys/msg.h>

int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

## Arguments

- **msqid**
The message queue descriptor.
&nbsp;
- **msgp**
A user-defined buffer that contains sending message information. The buffer contains first a field of type *long* specifying the type of the message, and then a data portion that holds the data bytes of the message.
&nbsp;
- **msgsz**
The length of the message text.
&nbsp;
- **msgflg**
Flags that control the operation of the call.

## Library
libc

## Description
The msgsnd() function sends a message to the queue associated with the message queue identifier specified by *msqid*.
&nbsp;
If there is no sufficient space available in the queue, message sending operation will block by default.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EACCES**
Operation permission is denied to the calling process.
&nbsp;
- **EAGAIN**
The message cannot be sent due to the *msg_qbytes* limit for the queue and (*msgflg* & IPC_NOWAIT) is non-zero.
&nbsp;
- **EFAULT**
The address pointed to by *msgp* is not accessible.
&nbsp;
- **EIDRM**
The message queue identifier *msqid* is removed from the system.
&nbsp;
- **EINTR**
The msgsnd() function was interrupted by a signal.
&nbsp;
- **EINVAL**
The value of *msqid* is not a valid message queue identifier, or the value of *mtype* is nonpositive; or the value of *msgsz* is greater than the system-imposed limit.
&nbsp;
- **ENOMEM**
The system does not have enough memory to support sending message operation.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
