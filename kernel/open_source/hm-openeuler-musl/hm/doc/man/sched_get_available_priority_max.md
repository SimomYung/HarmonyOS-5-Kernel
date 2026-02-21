# sched\_get\_available\_priority\_max()
Get the max available priority of the current thread

## Synopsis
```c
#include <hmsched.h>

int sched_get_available_priority_max();
```

## Arguments
N/A

## Library
libc

## Description
Get the max available priority value of the current thread according to the security permission. And this value is the max priority that can be set by the current thread.

Only supported when the thread is an RT scheduling strategy.

## Returns

- **n**
When *n* is in [1,99], success. *n* is the max available priority value of the current thread.
&nbsp;

- **-1**
An error occurred.

## Errors

- **ENOSYS**
The current thread is not a RT scheduling strategy.

## Examples
&nbsp;

## Classifications
Native Interface

## Function Safety
TODO
