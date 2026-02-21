# mq_send(), mq_timedsend()
Send a message to a message queue

## Synopsis
```c
#include <mqueue.h>

int mq_send(mqd_t mqd, const char *msg, size_t len, unsigned prio)

#include <time.h>
#include <mqueue.h>

int mq_timedsend(mqd_t mqd, const char *restrict msg, size_t len,
		 unsigned restrict prio, const struct timespec *restrict at);
```

## Arguments

- **mqd**
The message queue descriptor.
&nbsp;
- **msg**
The buffer used to send messages.
&nbsp;
- **len**
The size of the buffer pointed to by *msg*.
&nbsp;
- **prio**
The priority of the sending message.
&nbsp;
- **at**
A pointer that points to a structure which specifies how long the call will block.

## Library
libc

## Description
mq_send() adds the message pointed to by *msg* to the message queue referred to by the message queue descriptor *mqd*. The *len* argument specifies the length of the message pointed to by *msg*; this length must be less than or equal to the queue's mq_msgsize attribute. Zero-length messages are allowed.
&nbsp;

The *prio* argument is a nonnegative integer that specifies the priority of this message.  Messages are placed on the queue in decreasing order of priority, with newer messages of the same priority being placed after older messages with the same priority.
&nbsp;

If the message queue is already full (i.e., the number of messages on the queue equals the queue's *mq_maxmsg* attribute), then, by default, mq_send() blocks until sufficient space becomes available to allow the message to be queued, or until the call is interrupted by a signal handler.  If the O_NONBLOCK flag is enabled for the message queue description, then the call instead fails immediately with the error EAGAIN.
&nbsp;

Message queue descriptors will not be inherited by the child process.
&nbsp;

mq_timedsend() behaves just like mq_send(), except that if the queue is full and the O_NONBLOCK flag is not enabled for the message queue description, then *at* points to a structure which specifie how long the call will block.
&nbsp;

This value is an absolute timeout in seconds and nonoseconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC), specified in the following structure:
```c
struct timespec {
    time_t tv_sec;        /* seconds */
    long   tv_nsec;       /* nanoseconds */
};
```

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EAGAIN**
The queue was full, and the O_NONBLOCK flag was set for the message queue description referred to by *mqd*.
&nbsp;
- **EBADF**
The descriptor specified in *mqd* was invalid or not opened for writing.
&nbsp;
- **EINTR**
The call was interrupted by a signal handler.
&nbsp;
- **EMSGSIZE**
*len* was greater than the *mq_msgsize* attribute of the message queue.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO
