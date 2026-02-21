# pause()
Suspend the thread until a signal is received

## Synopsis
```c
#include <unistd.h>

int pause(void);
```

## Arguments
N/A

## Library
libc

## Description

The pause() function suspends the calling thread until a signal whose action is either to execute a signal-catching function or to terminate the process received.
&nbsp;
If the action is to terminate the process, pause() will not return. If the action is to execute a signal-catching function, pause() will return after the signal-catching function returns.

## Returns

- **no return**
The pause() function suspends thread execution indefinitely, so there is not successful return.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EINTR**
The calling process caught a signal and control is returned from the signal-catching function.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
