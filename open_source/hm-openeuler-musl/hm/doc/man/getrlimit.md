# getrlimit(), setrlimit()
Get/set resource limits

## Synopsis
```c
#include <sys/resource.h>

int getrlimit(int resource, struct rlimit *rlim);
int setrlimit(int resource, const struct rlimit *rlim);
```

## Arguments

- **resource**
The resource type you want to access.
&nbsp;
- **rlim**
The resource limit to be get or set.

## Library
libc

## Description
The function can be used to get/set the resource limits. each resource has an associated soft and hard limit. as defined by the rlimit structure:
  ```C
    struct rlimit {
        rlim_t rlim_cur;  /* Soft limit */
        rlim_t rlim_max;  /* Hard limit */
    };
 ```
 &nbsp;

the soft limit is value that enforces on the corresponding resource, and the hard limit is a ceiling for the soft limit. only the process with ab_rlimit and **POSIX_CAP_SYS_RESOURCE** can up the hard limit.
&nbsp;

The value RLIM_INFINITY denotes no limit on a resource.
&nbsp;

The resource type must be one of:
  - RLIMIT_AS

    The maximum size of process virtual memory in bytes. the affects brk(), mmap(), mremap(), which fail with error ENOMEM upon exceeding this limit.

  - RLIMIT_CORE

    the maximum size of a core file, when 0 no core dump files are created.

  - RLIMIT_DATA

    The maximum size of the process's data segment (initialized data, uninitialized data, and heap). this limit affects calls to brk(), which fail with the error ENOMEM upon exceeding this limit.

  - RLIMIT_FSIZE

    The Maximum size of files the process may create. this limit affects the relevant calls which attempts to extent a file beyond this limit (e.g., to write(), truncate()) fails with the error EFBIG.

  - RLIMIT_MEMLOCK

    The maximum size of memory that may be locked into RAM. this limit affects mlock() and mlockall() and the mmap() MAP_LOCKED, no limits are placed on the process with **POSIX_CAP_IPC_LOCK** capability.

  - RLIMIT_MSGQUEUE

    The maximum size of bytes that cal be allocated for POSIX message queues for the real user ID of the calling process. this limit enforced for mq_open(3). Each message create count against this formula:
    ```C
      bytes = attr.mq_maxmsg *
              (attr.mq_msgsize + sizeof(struct mqueue_msg))
    ```

  - RLIMIT_NOFILE

    The maximum of opened file descriptor of the calling process. Attempts (e.g. open(), pipe(), dup()) to exceed this limit yield the error EMFILE.

  - RLIMIT_NPROC

    The maximum of process can be created for the real user ID of the calling process. this limit affects calls to fork(), execve() which fail with the error EAGAIN upon exceeding this limit. no limits are placed on the process either with **POSIX_CAP_SYS_ADMIN** or **POSIX_CAP_RESOURCE** capability.

  - RLIMIT_RTPRIO

    The ceiling real-time priority that may be set for this process using sched_setscheduler() and sched_setparam(), no limits are placed on the process with **POSIX_CAP_SYS_NICE** capability.

  - RLIMIT_NICE

    The ceiling nice value that may be set for this process using setpriority() and nice()(For more detail on the nice value, see nice()), no limits are placed on the process with **POSIX_CAP_SYS_NICE** capability.

  - RLIMIT_SIGPENDING

    The maximum of signals that may be queued for the real user ID of the calling process. The limit is enforced for the real time signals of the sigqueue(3) only.

  - RLIMIT_STACK

    The maximum size of the process stack, in bytes.  Upon reaching this limit, a SIGSEGV signal is  generated.

## Returns

- **0**
The system call success
&nbsp;
- **-1**
An error occurred.

## Errors

- **EFAULT**
Invalid address of *rlim*
&nbsp;
- **EINVAL**
Invalid resource type
&nbsp;
- **EPERM**
No permission to raise the hard limit

## Example

```C
#include <stdlib.h>
#include <sys/resource.h>

int main(int argc, char *argv[])
{
    int rc;
    struct rlimit rlim;

    rc = getrlimit(RLIMIT_NOFILE, &rlim);
    if (rc < 0) {
        exit(-1);
    }

    rlim.rlim_cur = 4096;
    rc = setrlimit(RLIMIT_NOFILE, &rlim);
    if (rc < 0) {
        exit(-2);
    }

    return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
The resources of RLIMIT\_MSGQUEUE, RLIMIT\_NPROC, RLIMIT\_RTPRIO, RLIMIT\_SIGPENDING, RLIMIT\_STACK are restricted by CONFIG\_RLIMIT\_MSGQUEUE, CONFIG\_RLIMIT\_NPROC, CONFIG\_RLIMIT\_RTPRIO, CONFIG\_RLIMIT\_SIGPENDING, CONFIG\_RLIMIT\_STACK separately. The set of these resources take no effect when the related config is off.

As musl will try prlimit() first, if *rlim* is NULL, setrlimit() will return 0 and getrlimit() will raise a segment fault.
