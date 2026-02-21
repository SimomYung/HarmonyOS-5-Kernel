# getcpu()
Determine CPU and NUMA node on which the calling thread is running

## Synopsis
```c
#include <sched.h>

int getcpu(unsigned int *cpu, unsigned int *node);
```

## Arguments

- **cpu**
The parameter *cpu* can be used to store the cpu value which one calling thread is running on.
&nbsp;
- **node**
The parameter *node* can be used to store the node value which one calling thread is running on.
&nbsp;

## Library
libc

## Description
The function getcpu() can be used to get which CPU and NUMA node the thread is running on.

## Returns

- **0**
Success.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EFAULT**
Arguments point outside the calling process's address space.

## Example
&nbsp;

## Classification
Unix

## Function Safety
N/A
