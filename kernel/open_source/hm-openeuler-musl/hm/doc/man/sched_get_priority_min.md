# sched\_get\_priority\_min()
Get the minimum priority value of a scheduling policy

## Synopsis
```c
#include <sched.h>

int sched_get_priority_min(int policy);
```

## Arguments

- **policy**
Identify which scheduling algorithm is in use.

## Library
libc

## Description
Get the minimum priority value of a scheduling policy's algorithm. *policy* is used to identify the scheduling algorithm. A process with higher priority value will be scheduled earlier than those with lower priority values.

SCHED_FIFO, SCHED_RR, SCHED_NORMAL are supported. The minimum priority of FIFO and RR is 1. The minimum priority of SCED_NORMAL is 0.

## Returns

- **n**
When *n* is not -1, success. *n* is the minimum priority value of the scheduling policy.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EINVAL**
The policy identified by *policy* can not be found.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
