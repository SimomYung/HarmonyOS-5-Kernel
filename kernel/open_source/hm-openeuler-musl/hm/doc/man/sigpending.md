# sigpending()
Acquire the pending signal set

## Synopsis
```c
#include <signal.h>

int sigpending(sigset_t *set);
```

## Arguments

- **set**
The signal set which the function returns.

## Library
libc

## Description
The sigpending() function returns the set of signals that are pending for delivery to the calling thread.
&nbsp;
The standard signals and real-time signals are currently supported.
&nbsp;
Note that you should pass a valid *set* to this function, invalid *set* (such as -1) may cause segmentation fault.
&nbsp;
When there is no thread in the process that can handle the signal, the signal is placed in sharepending(on process).
&nbsp;
All threads on this process obtain the pending signals through sigpending, it is the signals of pending on the thread and the signals in sharepending.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EFAULT**
*set* points to the memory which is not a valid part of the process address space.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
