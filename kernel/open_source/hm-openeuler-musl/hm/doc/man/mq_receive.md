# mq_receive(), mq_timedreceive()
Receive a message from a message queue

## Synopsis
```c
#include <mqueue.h>

ssize_t mq_receive(mqd_t mqd, char *msg, size_t len, unsigned *prio);

#include <time.h>
#include <mqueue.h>

ssize_t mq_timedreceive(mqd_t mqd, char *restrict msg, size_t len,
			unsigned *restrict prio, const struct timespec *restrict at);
```

## Arguments

- **mqd**
The message queue descriptor.
&nbsp;
- **msg**
The buffer used to receive messages.
&nbsp;
- **len**
The size of the buffer pointed to by *msg*.
&nbsp;
- **prio**
The priority associated with the received message.
&nbsp;
- **at**
A pointer that points to a structure which specifies how long the call will block.

## Library
libc

## Description
mq_receive() removes the oldest message with the highest priority from the message queue referred to by the message queue descriptor *mqd*, and places it in the buffer pointed to by *msg*. The *len* argument specifies the size of the buffer pointed to by *msg*; this must be greater than or equal to the *mq_msgsize* attribute of the queue (see mq_getattr()).  If *prio* is not NULL, then the buffer to which it points is used to return the priority associated with the received message.
&nbsp;

If the queue is empty, then, by default, mq_receive() blocks until a message becomes available, or the call is interrupted by a signal handler. If the O_NONBLOCK flag is enabled for the message queue
description, then the call instead fails immediately with the error EAGAIN.
&nbsp;

Message queue descriptors will not be inherited by the child process.
&nbsp;

mq_timedreceive() behaves just like mq_receive(), except that if the queue is empty and the O_NONBLOCK flag is not enabled for the message queue description, then *at* points to a structure which specifies how long the call will block.
&nbsp;

This value is an absolute timeout in seconds and nonoseconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC), specified in the following structure:
```c
struct timespec {
    time_t tv_sec;        /* seconds */
    long   tv_nsec;       /* nanoseconds */
};
```

## Returns

- **The number of bytes in the received message**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EAGAIN**
The queue was empty, and the O_NONBLOCK flag was set for the message queue description referred to by *mqd*.
&nbsp;
- **EBADF**
The descriptor specified in *mqd* was invalid or not opened for reading.
&nbsp;
- **EINTR**
The call was interrupted by a signal handler.
&nbsp;
- **EMSGSIZE**
*len* was less than the *mq_msgsize* attribute of the message queue.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO
