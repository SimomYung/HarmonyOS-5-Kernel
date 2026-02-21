# fork()
Create a new process

## Synopsis
```c
#include <unistd.h>

pid_t fork(void);
```
## Arguments

N/A

## Library
libc

## Description

The fork() function shall create a new process, which is called child process. The calling process is the parent process of the child process. A child process is an exact copy of its parent except for some attributes. The details of similarities and differences between the child process and the parent process are as follows:

* The child process will be assigned a unique process ID, which is different from any other process ID and process group ID. The parent process ID is the process ID of calling process.

* The child process will copy parent's file descriptors for itself. Every child's file descriptors refers to the same open file descriptors with its copy in parent.

* The child process will copy parent's open directory streams for itself. Every child's open directory stream has the same stream positioning with its copy in parent.

* The values of tms\_utime, tms\_stime, tms\_cutime, and tms\_cstime of child process shall be set to 0.

* The left time of alarm clock will be reset to zero, and the alarm will be canceled if there is any.

* The set of signals pending for the child process will be initialized to the empty set.

* Interval timers shall be reset in the child process.

* Semaphores will not be inherited by the child process.

* Memory mappings created by parent process will be inherited by child process. Before fork returns, any modification to MAP\_PRIVATE mappings data created by parent is visible to child. It will be invisible to each other after fork returns.

* For the SCHED\_FIFO and SCHED\_RR scheduling policies, the child process shall inherit the policy and priority settings of the parent process.

* Per-process timers created by the parent shall not be inherited by the child process.

* Message queue descriptors will not be inherited by the child process.

* Asynchronous input and asynchronous output operations will not be inherited by the child process.

* The child process will be created with a single thread. The child process will contain a replica of the parent thread and its entire address space, possibly including the states of mutexes and other resources. After that, to avoid errors, the child process may only execute async-signal-safe operations until such time as one of the exec functions is called. Fork handlers may be created by pthread\_atfork() function in order to maintain application invariants across fork() calls.

If CONFIG\_FORK\_COW in sysmgr configuration is enabled, fork() has a basic copy-on-write(cow) implementation. Instead of doing a full address space duplication during fork, cow doing page duplication on demand. This basic cow implementation has the following limitations:

* IO memory mapping is not supported in fork, i.e. child process cannot inherit IO mapping from parent process.

* Main stack, activation stack not doing cow by default.

## Returns

- **0**
When fork succeeds, 0 will be returned to child process.
&nbsp;
- **process ID of the child process**
When fork succeeds, the process ID of the child process will be returned to the parent process.
&nbsp;
- **-1**
-1 will be returned if fork failed.

## Errors

- **EAGAIN**
The RLIMIT\_NPROC soft resource limit, which limits the number of processes for a real user ID, is reached.
&nbsp;
- **ENOMEM**
Lacking memory to fork.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
