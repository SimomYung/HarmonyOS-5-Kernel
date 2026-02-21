# process\_vm\_writev()
Write data to target process address space

## Synopsis
```c
#include <sys/uio.h>

int process_vm_writev(pid_t pid,
                      const struct iovec *local_iov,
                      unsigned long liovcnt,
                      const struct iovec *remote_iov,
                      unsigned long riovcnt,
                      unsigned long flags);
```

## Arguments

- **pid**
The target process pid to write to.
&nbsp;
- **local_iov**
Local buffers to be written.
&nbsp;
- **liovcnt**
Local iov count.
&nbsp;
- **remote_iov**
Remote buffers to receive data.
&nbsp;
- **riovcnt**
Remote buffers count.
&nbsp;
- **flags**
For future usage.


## Library
libc

## Description
The process\_vm\_writev() function write data from the address space of the calling process ("the local process") to the process identified by *pid* ("the remote process"). The data moves directly between the address spaces of the two processes, without passing through kernel space.

## Returns

On success, the function return the number of bytes written and this number may be less than
the number specified by caller.
&nbsp;

On failure, -1 is returned.
## Errors

- **EACCES**
The caller must be in same group with the target process and has the process\_vm\_access ability.
&nbsp;
- **EFAULT**
The liov or riov buffer is invalid or not accessible.
&nbsp;
- **EINVAL**
The flags is not zero or the length specified by iov is too large.
&nbsp;
- **ESRCH**
The process specified by pid does not exist.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
