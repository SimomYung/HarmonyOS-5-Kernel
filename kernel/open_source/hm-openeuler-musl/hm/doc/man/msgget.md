# msgget()
Get a System V message queue identifier

## Synopsis
```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgget(key_t key, int msgflg);
```

## Arguments

- **key**
Key value that is used to create or open a System V message queue.
&nbsp;
- **msgflg**
Flag that controls the operation of the call.

## Library
libc

## Description
The msgget() function returns the System V message queue identifier associated with the value of the *key* argument.  It may be used either to obtain the identifier of a previously created message queue when *msgflg* is zero and *key* does not have the value IPC_PRIVATE, or to create a new set.
&nbsp;
A new message queue is created if *key* has the value IPC_PRIVATE or no message queue with the given key *key* exists, and IPC_CREAT is specified in *msgflg*.
&nbsp;
If *msgflg* specifies both IPC_CREAT and IPC_EXCL and a message queue already exists for *key*, then msgget() fails with errno set to EEXIST.

## Returns

- **A message queue descriptor**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EACCES**
A message queue exists for *key*, but the calling process does not have permission to access the queue.
&nbsp;
- **EEXIST**
IPC_CREAT and IPC_EXCL were specified in *msgflg*, but a message queue already exists for *key*.
&nbsp;
- **ENOENT**
No message queue exists for *key* and *msgflg* did not specify IPC_CREAT.
&nbsp;
- **ENOMEM**
A message queue has to be created but the system does not have enough memory for the new data structure.
&nbsp;
- **ENOSPC**
A message queue has to be created but the system limit for the maximum number of message queues (MSGMNI) would be exceeded.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
