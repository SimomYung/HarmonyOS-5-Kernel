# dup(), dup2(), dup3()
Duplicate a file descriptor

## Synopsis
```c
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>

int dup3(int oldfd, int newfd, int flags);
```

## Arguments

- **oldfd**
The old file descriptor specified to be copied from.
&nbsp;
- **newfd**
The new file descriptor specified for copying from *oldfd*.
&nbsp;
- **flags**
Only O\_CLOEXEC is supported for this argument.

## Library
libc

## Description
dup() copies from the file descriptor *oldfd* and returns a new file descriptor, the old and new file descriptors may be used alternately. They refer to the same open file description and share file offset and file status flags. The two descriptors do not share file descriptor flags (the close-on-exec flag).
&nbsp;
dup2() performs the same task as dup(), but instead of using the new allocated file descriptor, it uses the descriptor number specified in *newfd*. If the descriptor *newfd* was opened , it is atomically closed before being reused.  If *newfd* is the same value as *oldfd* which is a valid file descriptor, then dup2() returns newfd. If *oldfd* is not a valid file descriptor, then dup2() fails and does nothing.
&nbsp;
dup3() performs the same task as dup2(), but dup3() can force the close-on-exec flag to be set for the new file descriptor by specifying O\_CLOEXEC in flags.

## Returns
On success, these functions return the new file descriptor. On error, -1 is returned, and errno is set to indicate the error.
&nbsp;
## Errors
- **EBADF**
The argument *oldfd* or *newfd* is not a valid file descriptor.
&nbsp;
- **EINVAL**
The argument *oldfd* does not refer to a file. The argument *flags* is not set appropriately.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EMFILE**
The per-process limit on the number of open file descriptors has been reached.
&nbsp;
- **EFAULT**
Can not get the *newfd*.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
