# abort()
Generate an abnormal process abort

## Synopsis
```c
#include <stdlib.h>

void abort(void);
```

## Arguments
N/A

## Library
libc

## Description
The abort() function causes the calling process to terminate abnormally. The effect is as if calling raise(SIGABRT) in most cases. If the SIGABRT signal is ignored or caught by a signal handler that will return, the abort() function will uninstall any handler of SIGABRT and re-raise the signal to generate the default action of SIGABRT (abnormal termination).
&nbsp;
Calling wait()/waitpid()/waitid() on the aborted process will get a status which indicates that the target process is terminated by the signal SIGABRT.

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
