# exit()
Terminate the calling process

## Synopsis
```c
#include <stdlib.h>

void exit(int status);
```

## Arguments

- **status**
Exit status.

## Library
libc

## Description
The exit() function terminates the current process, all active threads in the process are killed. *status* is regarded as the exit status (only the lower 16 bits will be used, that is status & 0xFF) of the calling process, and it can be obtained by a successful wait operation on this process.
&nbsp;
An implicit call to exit() is made when the main thread of the calling process first invokes return, the return value of main() will be passed as *status*.
&nbsp;
All functions registered with atexit() are called in the reverse order of their registration.

## Returns
N/A

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
