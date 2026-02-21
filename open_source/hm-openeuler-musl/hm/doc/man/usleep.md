# usleep()
Suspend execution for the specified number of microseconds

## Synopsis
```c
#include <unistd.h>

int usleep(unsigned usec);
```

## Arguments
- **usec**
Number of seconds.

## Library
libc

## Description
The usleep() function makes the calling thread sleep for (at least) *usec* microseconds.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred or interrupted by a signal.

## Errors
- **EINTR**
Interrupted by a signal.
&nbsp;
- **EINVAL**
*usec* is not smaller than 1000000.

## Example
&nbsp;
## Classification
Unix

## Function Safety
TODO
