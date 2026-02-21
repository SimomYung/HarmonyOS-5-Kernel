# fcntl()
Perform variety of operations on the file

## Synopsis
```c
#include <fcntl.h>
#include <unistd.h>

int fcntl(int fd, int cmd, ...);
```

## Arguments

- **fd**
The file descriptor relates to the file (will be operated).
&nbsp;
- **cmd**
Variety of commands are supported in fcntl() function, more details see Description.

## Library
libc

## Description
When fd is a file descriptor of filesystem, only command F\_DUPFD, F\_GETFD, F\_SETFD, F\_DUPFD\_CLOEXEC, F\_SETFL, F\_GETFL, F\_SETLK, F\_SETLKW, F\_GETLK, F\_SETPIPE\_SZ and F\_GETPIPE\_SZ are supported. More specifically, F\_DUPFD, duplicates the file descriptor *fd* using the highest-numbered available file descriptor greater than or equal to the input arguments.
&nbsp;
F\_DUPFD\_CLOEXEC, same with F\_DUPFD, but additionally sets the close-on-exec flag for the duplicate file descriptor.
&nbsp;
F\_SETFL, set the file status flags to the value specified by input arguments.
&nbsp;
When fd is a socket, only O\_NONBLOCK and O\_RDWR are supported for arguments.
&nbsp;
When fd is a file descriptor of filesystem, only O\_RDONLY , O\_WRONLY, O\_RDWR , O\_APPEND , O\_NOFOLLOW, O\_NONBLOCK are supported for arguments.
&nbsp;
When fd is a socket, only command F\_GETFL, F\_SETFL, F\_DUPFD, F\_DUPFD\_CLOEXEC, F\_GETFD and F\_SETFD are supported.
&nbsp;
Only flag FD\_CLOEXEC is defined if *cmd* is F\_GETFD or F\_SETFD while only flag O\_RDWR and O\_NONBLOCK are defined if *cmd* is F\_GETFL or F\_SETFL.
&nbsp;
When F\_SETLK, F\_SETLKW, F\_GETLK, F\_SETLK64, F\_GETLK64 is specified, a *struct flock* is used as the third parameter to describe the lock which is required. This lock is an Advisory File Lock, caller must acquire lock explicitly. A file or a range in the file could be locked.
&nbsp;
When fd is created by memfd\_create syscall with flag MFD\_ALLOW\_SEALING, command F\_ADD\_SEALS and F\_GET\_SEALS is supported.
F\_ADD\_SEALS
	Add seals to the file, and seals can never be removed.
F\_GET\_SEALS
	Get the current set of seals of the inode referred to by fd.
Detailed parameters about F\_ADD\_SEALS are as following:
	F\_SEAL\_SEAL   prevent further seals from being set
	F\_SEAL\_SHRINK prevent file from shrinking
	F\_SEAL\_GROW   prevent file from growing
	F\_SEAL\_WRITE  prevent file from writing
	F\_SEAL\_FUTURE\_WRITE  prevent file from future writing
&nbsp;
When *fd* is a file descriptor of a pipe, the capacity of the pipe can be changed with command F\_SETPIPE\_SZ and F\_GETPIPE\_SZ.
F\_SETPIPE\_SZ
	Change the capacity of the pipe referred to by *fd* to be *arg* bytes. An unprivileged process can adjust the pipe capacity to a value between the page size and the limit defined in */proc/sys/fs/pipe-max-size*, if it attempts to set the pipe capacity above the limit, the error EPERM is returned. A privileged process (**POSIX_CAP_SYS_RESOURCE**) can override the limit. If *arg* is below the page size, the capacity of the pipe will be silently rounded up to the page size. Otherwise the capacity of the pipe will be set as the next higher power-of-two page-size multiple of *arg*, and *arg* cannot be larger than 67108864 bytes (64MB) or ENOMEM is returned. If attempting to set the pipe capacity smaller than the amount of buffer space currently used, the error EBUSY is returned.
F\_GETPIPE\_SZ
	Return the capacity of the pipe referred to by *fd*.
&nbsp;

## Returns
The return values of this function depend on the operations, such as:
&nbsp;
- **F_DUPFD**
Perform duplication operation and returns the new fd.
&nbsp;
- **F_GETFD**
The value of fd flags.
&nbsp;
- **F_GETFL**
The value of file flags.
&nbsp;
- **F_GETPIPE_SZ**
The capacity of the pipe referred to by *fd*.
&nbsp;
- **F_SETPIPE_SZ**
The new capacity of the pipe referred to by *fd*.
&nbsp;
- **0**
The set functions are finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors

- **EBADF**
The *fd* is invalid.
&nbsp;
- **EINVAL**
This error will happen when the input arguments and *cmd* conflict with each other.
&nbsp;
- **ENOMEM**
There is not enough memory during the function processing (unlikely to happen).
&nbsp;
*cmd* is F\_SETPIPE\_SZ and *arg* is larger than 67108864 bytes (64MB).
&nbsp;
- **ENOLCK**
Cannot get internal lock structures.
&nbsp;
- **EDEADLK**
The requested lock will trigger deadlock with existing locks.
&nbsp;
- **ENOSYS**
Open File Descriptor lock is not supported for now or The *fd* that belong to devfs file does not support operation with *cmd* F\_SETFL, F\_GETFL.
&nbsp;
- **EAGAIN**
A nonblock lock is requested and could be blocked by existing lock or The operation is prohibited because the file has been memory-mapped by another process.
&nbsp;
- **EINTR**
Blocked lock is interrupted by signal.
&nbsp;
- **EBUSY**
*cmd* is F\_ADD\_SEALS, *arg* includes F\_SEAL\_WRITE, and there exists a shared mapping on the file referred to by *fd*.
&nbsp;
*cmd* is F\_SETPIPE\_SZ and the new capacity of pipe specified in *arg* is smaller than the amount of buffer space currently used to store data.
&nbsp;
- **EPERM**
*cmd* is F\_SETPIPE\_SZ and attempting by an unprivileged process to set the pipe capacity above the limit in */proc/sys/fs/pipe-max-size*.
&nbsp;
- **EFAULT**
*lock* is an outside address.
&nbsp;
- **EMFILE**
Cmd is F_DUPFD and the process limit on the number of open file descriptors has been reached.
## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
Lock related *cmd*s are not enabled when system is compiled with CONFIG\_FS\_FLOCK off. Calling the API under this condition will return ENOSYS.
