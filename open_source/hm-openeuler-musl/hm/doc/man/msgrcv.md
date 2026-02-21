# msgrcv()
XSI message receive operation

## Synopsis
```c
#include <sys/msg.h>

ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```

## Arguments

- **msqid**
The message queue descriptor.
&nbsp;
- **msgp**
A user-defined buffer used to receive sending messages. The buffer contains first a field of type *long* specifying the type of the message, and then a data portion that holds the data bytes of the message.
&nbsp;
- **msgsz**
The size in bytes of *mtext*.
&nbsp;
- **msgtyp**
Specifies the type of message requested.
&nbsp;
- **msgflg**
Specifies the action to be taken if a message of the desired type is not on the queue. Note that MSG_COPY flag is not supported.

## Library
libc

## Description
The msgrcv() function reads a message from the queue associated with the message queue identifier specified by *msqid* and places it in the user-defined buffer pointed to by *msgp*.
&nbsp;
The application shall ensure that the argument *msgp* points to a user-defined buffer that contains first a field of type long specifying the type of the message, and then a data portion that holds the data bytes of the message. The structure below is an example of what this user-defined buffer might look like:
&nbsp;
```c
	struct mymsg {
		long    mtype;     /* Message type. */
		char    mtext[1];  /* Message text. */
	}
```
&nbsp;
The structure member *mtype* is the received message's type as specified by the sending process.
&nbsp;
The structure member *mtext* is the text of the message.

## Returns

- **The number of bytes actually placed into the buffer mtext**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **E2BIG**
The value of *mtext* is greater than *msgsz* and (*msgflg* & MSG_NOERROR) is 0.
&nbsp;
- **EACCES**
Operation permission is denied to the calling process.
&nbsp;
- **EFAULT**
The address pointed to by *msgp* is inaccessible.
&nbsp;
- **EIDRM**
The message queue identifier *msqid* is removed from the system.
&nbsp;
- **EINTR**
The msgrcv() function was interrupted by a signal.
&nbsp;
- **EINVAL**
*msqid* is not a valid message queue identifier or *msgsz* is less than 0.
&nbsp;
- **ENOMEM**
The system does not have enough memory to support receiving message operation.
&nbsp;
- **ENOMSG**
The queue does not contain a message of the desired type and (*msgflg* & IPC_NOWAIT) is non-zero.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
