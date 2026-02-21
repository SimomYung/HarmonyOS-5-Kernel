# epoll\_ctl()
Add, modify, or remove entries in the interest list of epoll instance

## Synopsis
```c
#include <sys/epoll.h>

int epoll_ctl(int fd, int op, int fd2, struct epoll_event *ev);
```

## Arguments

- **fd**
The file descriptor of the source file where the operations will be performed and it refers to an epoll instance.
&nbsp;
- **op**
The types of operations, such as:
&nbsp;
    - **EPOLL_CTL_ADD**
        Add the target(*fd2*) to the epoll instance(*fd*) according to the event described in event *ev*.
&nbsp;
    - **EPOLL_CTL_MOD**
        Modify the target(*fd2*).
&nbsp;
    - **EPOLL_CTL_DEL**
        Delete the target(*fd2*) from the epoll instance(*fd*). In such situation, The *ev* is ignored.
&nbsp;
- **fd2**
The file descriptor of the target file (or device).
&nbsp;
- **ev**
The *struct epoll\_event* is defined as:
```
    typedef union epoll_data {
        void        *ptr;
        int          fd;
        uint32_t     u32;
        uint64_t     u64;
    } epoll_data_t;

    struct epoll_event {
        uint32_t     events;      /* Epoll events */
        epoll_data_t data;        /* User data variable */
    };
```
&nbsp;
The *data* member of the *struct epoll\_event* specifies data that will be saved here and then return (see epoll\_wait) when this file descriptor becomes ready.
The *events* member of the *epoll\_event* structure is a bit mask composed by ORing together zero or more of the following available event types:
&nbsp;
    - **EPOLLIN**
        The associated file is available for read operations.
&nbsp;
    -  **EPOLLOUT**
        The associated file is available for write operations.
&nbsp;
    -  **EPOLLERR**
    Error condition happened on the associated file descriptor.
&nbsp;
    -  **EPOLLHUP**
        Hang up happened on the associated file descriptor.
        When reading from a channel such as a pipe or a stream socket, this event merely indicates that the peer closed its end of the channel.
&nbsp;
    -  **EPOLLET**
        Requests edge-triggered notification for the associated file descriptor. The default behavior for epoll is level-triggered. See *Level-triggered and edge-triggered* section for more details.
&nbsp;
    See *Supported file descriptor* section for more details about supported fd and event.

## Library
libc

## Description

- **Level-triggered and edge-triggered**
The epoll event distribution interface is able to behave both as edge-triggered (ET) and as level-triggered (LT).
The difference between the two mechanisms can be described as follows. Suppose that this scenario happens:
1. The file descriptor that represents the read side of a pipe (rfd) is registered on the epoll instance.
1. A pipe writer writes 2 kB of data on the write side of the pipe.
1. A call to epoll\_wait is done that will return rfd as a ready file descriptor.
1. The pipe reader reads 1 kB of data from rfd.
1. A call to epoll\_wait is done.
&nbsp;
If the rfd file descriptor has been added to the epoll interface using the EPOLLET (edge-triggered) flag, the call to epoll\_wait done in step 5 will probably hang despite the available data still present in the file input buffer; meanwhile the remote peer might be expecting a response based on the data it already sent.
&nbsp;
The reason for this is that edge-triggered mode delivers events only when changes occur on the monitored file descriptor. So, in step 5 the caller might end up waiting for some data that is already present inside the input buffer.
&nbsp;
In the above example, an event on rfd will be generated because of the write done in 2 and the event is consumed in 3.  Since the read operation done in 4 does not consume the whole buffer data, the call to epoll\_wait done in step 5 might block indefinitely.
&nbsp;
By contrast, when used as a level-triggered interface (the default, when EPOLLET is not specified), epoll\_wait in step 5 will return.
&nbsp;
&nbsp;
- **Supported file descriptor**
Epoll is implemented according to linux, but not exactly same as epoll of linux. Here is the list of supported file descriptors and events.
    - **eventfd**
    EPOLLIN/EPOLLOUT/EPOLLERR are supported.
    Watching eventfd by epoll will be inherated after fork.

    - **pipe**
    EPOLLIN/EPOLLOUT/EPOLLRDNORM/EPOLLWRNORM/EPOLLERR/EPOLLHUP are supported.
    Watching pipe by epoll will be inherated after fork.

    - **signalfd**
    EPOLLIN is supported.
    Watching signalfd by epoll will be inherated after fork, but epoll\_wait may not return accurate events. A possible workaround is that after the fork, the child process can close the signalfd file descriptor that it inherited from the parent process and then create another signalfd file descriptor and add it to the epoll instance.

    - **timerfd**
    EPOLLIN is supported. Watching timerfd by epoll will be inherated after fork.

    - **kmsg**
    EPOLLIN is supported. Watching kmsg by epoll will be inherated after fork.

    - **inotify**
    EPOLLIN is supported. Watching inotify by epoll will be inherated after fork.

    - **fanotify**
    EPOLLIN is supported. Watching fanotify by epoll will be inherated after fork.

    - **unix socket**
    EPOLLIN/EPOLLOUT/EPOLLHUP are supported. Watching unix socket by epoll will be inherated after fork. However, if the parent process closes the socket or exits right after fork, the watching may lost in child process.

    - **tty**
    EPOLLIN, EPOLLOUT and EPOLLHUP are supported. Watching tty by epoll will be inherated after fork.

    - **/dev/random & /dev/urandom**
    EPOLLIN is supported. Watching random devices by epoll will be inherated after fork. However, if the parent process closes the fd or exits right after fork, the watching may lost in child process.

    - **epoll fd**
    EPOLLIN is supported. Watching epoll by epoll. Epoll fd can only be watched by epoll, and the nested depth can only be 1.

    - **other fd**
    Not specifically descriped here. Please refer to the document for the fd.
&nbsp;
&nbsp;
- **Working with other syscall**
    - **exec**
    Epoll fd marked EPOLL\_CLOEXEC will be closed automatically when calling exec. See *epoll\_create1()* for more details.

    - **fork**
    The child will inherit the parent's epoll fd and all the watched fds. See "Supported file descriptor" section for more details. When either of the parent or child processes is modified by epoll\_ctl(), the other process is modified synchronously.

    - **dup**
    1) Dup an epoll fd is supported.
    2) Dup to an epoll fd will close the fd silently.
    3) Add a duplicate fd to the same epoll instance is supported. This can be a useful technique for filtering events, if the duplicate file descriptors are registered with different events masks.

    - **close**
    After closing fd watched by epoll, the fd will be removed from epoll silently, even if the fd was duplicated via dup or fork, the duplicated fd will not influence the epoll. Poll and select have the same effect.

## Returns

- **0**
The function completes successfully and return zero.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.
&nbsp;

## Errors

- **EFAULT**
*ev* is an invalid address
&nbsp;
- **EPERM**
*fd2* doesn't support being watched by epoll. This error can occur if *fd2* refers to, for example, a regular file or a directory.
&nbsp;
- **EBADF**
*fd* or *fd2* is not a valid file descriptor.
&nbsp;
- **EINVAL**
*fd* is not an epoll file descriptor.
*fd2* is an epoll file descriptor.
*op* is invalid.
&nbsp;
- **EEXIST**
The *op* is EPOLL\_CTL\_ADD, and the target *fd2* has already been added to *fd*.
&nbsp;
- **ENOENT**
The *op* is EPOLL\_CTL\_MOD or EPOLL\_CTL\_DEL, and *fd2* has not been registered with this epoll instance.
&nbsp;
- **ENOMEM**
No enough memory during the process (unlikely).
- **ELOOP**
*fd2* refers to an epoll instance and this EPOLL_CTL_ADD operation would result in a nesting depth of epoll instances greater than 1.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
