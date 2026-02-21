# sleep()
Suspend execution for the specified number of seconds

## Synopsis
```c
#include <unistd.h>

unsigned int sleep(unsigned seconds);
```

## Arguments
- **seconds**
Number of seconds.

## Library
libc

## Description
The sleep() function makes the calling thread sleep until *seconds* seconds have elapsed or a signal arrives.

## Returns
If the requested time has elapsed the sleep() function returns 0. If the call was interrupted by a signal, it returns the number of seconds left to sleep.

## Errors
N/A

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO