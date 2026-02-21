# clone()
Create a child process

## Synopsis
```c
#define _GNU_SOURCE
#include <sched.h>

int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...
        /* pid_t *parent_tid, void *tls, pid_t *child_tid */ );
```
## Arguments

- **fn**
Subprocess execution function.
&nbsp;
- **stack**
Subprocess running stack space.
&nbsp;
- **flags**
Flags bit mask.
&nbsp;
- **arg**
Input parameters of the subprocess execution function.
&nbsp;
- **parent_tid**
Memory used by the parent process to store the child process TID.
&nbsp;
- **tls**
Location of new TLS.
&nbsp;
- **child_tid**
Memory used by the child process to store the child process TID.
&nbsp;

## Library
libc

## Description
The process created by the parent process starts running from fn. When fn is complete, the child process exits.
- **The child termination signal**
When a child process exits, a signal may be sent to the parent process. The signal can be set by the lower 16 bits of flags. When this signal is not SIGCHLD, the parent process mustspecify __WALL or __WCLONE when waiting for the child process. When no signal is specified, the child process does not send a signal to the parent process.
&nbsp;

- **CLONE_CHILD_CLEARTID**
The child thread ID at the child\_tid in child memory will be cleared when the child exits, and the calling process will be awakened on the futex at that child\_tid address. Normally, it is used by thread libraries.
&nbsp;
- **CLONE_CHILD_SETTID**
Save the child thread ID at the child\_tid in the child's memory.
&nbsp;
- **CLONE_PAREANT_SETTID**
Save the child thread ID at the parent\_tid in the parent's memory.
&nbsp;
- **CLONE_PARENT**
If CLONE\_PARENT is set, then the child's parent is the calling process's parent. And when the child exits, calling process's parent will be signaled.
If CLONE\_PARENT is not set, then the child's parent is the calling process.
&nbsp;
- **CLONE_NEWPID**
If CLONE\_NEWPID is set, then create child process in a new PID namespace.
&nbsp;

## Returns

- **thread ID of the child process**
When clone succeeds, the thread ID of the child process will be returned to the parent process.
&nbsp;
- **-1**
-1 will be returned if clone failed.

## Errors

- **EINVAL**
CLONE\_THREAD was specified in the flags mask, but CLONE\_SIGHAND was not.
CLONE\_SIGHAND was specified in the flags mask, but CLONE\_VM was not.

&nbsp;
- **EAGAIN**
The RLIMIT\_NPROC soft resource limit, which limits the number of processes for a real user ID, is reached.
&nbsp;
- **ENOMEM**
Lacking memory to clone.
&nbsp;
- **EINVAL**
CLONE\_NEWPID and CLONE\_PARENT were specified at the same time.
&nbsp;
- **EINVAL**
CLONE\_NEWPID was specified, but the kernel was not configured with CONFIG\_PID\_NS.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
