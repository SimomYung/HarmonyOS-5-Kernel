# eventfd()
Generate a fd for an event notification

## Synopsis
```c
#include <sys/eventfd.h>
#include <unistd.h>

int eventfd(unsigned int count, int flags);
```

## Arguments
- **count**
To initialize the counter of the eventfd object (created by the function).
&nbsp;
- **flags**
This parameter specifies different types of behavior of function, such as:
EFD\_CLOEXEC, set the close-on-exec behavior on the fd (returned by the function).
EFD\_NONBLOCK, set the O\_NONBLOCK status on the returned fd.
EFD\_SEMAPHORE, give the semaphore similar semantics for the returned fd reading operation.

## Library
libc

## Description
The eventfd() function will generate a new fd which points to an eventfd object, several types of operations can be supported according to the input parameters.

## Returns
- **Positive number**
The fd (file descriptor) of the created eventfd object.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.

## Errors
- **EINVAL**
The input *flags* is invalid.
&nbsp;
- **EMFILE**
Error occurs befaces of RLIMIT\_NOFILE reject.
&nbsp;
- **ENOMEM**
There is not enough memory during the operation (unlikely to happen).
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
