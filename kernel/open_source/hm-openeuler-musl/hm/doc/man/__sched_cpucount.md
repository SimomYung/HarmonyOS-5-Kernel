# __sched_cpucount()
Get the number of CPUs by cpu_set_t data structure

## Synopsis
```c
#include <sched.h>

int __sched_cpucount(size_t size, const cpu_set_t *set);
```

## Arguments

- **size**
The size of the cpu_set_t.
&nbsp;
- **set**
The cpu_set_t data structure represents a set of CPUs.

## Library
libc

## Description
The function returns the number of CPUs by cpu_set_t data structure.

## Returns

The function returns the number of CPUs by cpu_set_t data structure.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
N/A
