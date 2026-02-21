# sched\_getaffinity()
Get the CPU affinity bits for a process

## Synopsis
```c
#include <sched.h>

int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);
```

## Arguments

- **pid**
Process ID.
&nbsp;
- **cpusetsize**
The size in bytes of *mask*.
&nbsp;
- **mask**
The presentation of the process's CPU affinity bits.


## Library
libc

## Description
Get the cpus that the process may run on. If *pid* is 0, get the calling process's affinity cpus.

## Returns

- **0**
Success.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EFAULT**
The address of mask is invalid.
&nbsp;
- **EINVAL**
*cpusetsize* is smaller than the size of the affinity mask used by the kernel.
&nbsp;
- **EPERM**
The caller process doesn't have enough privileges.
&nbsp;
- **ESRCH**
The process specified by *pid* can not be found.
&nbsp;
- **EACCES**
Fail to check Mandatory Access Control.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
