# mlockall(), munlockall()
Lock and unlock memory of the whole virtual address space

## Synopsis
```c
#include <sys/mman.h>

int mlockall(int flags);
int munlockall(void);
```

## Arguments

- **flags**
The flags to control the operation.


## Library
libc

## Description
The mlockall() function locks all virtual address space of the calling process into RAM, so that the memory will not be swapped out of RAM. The argument *flags* can be constructed as the bitwise OR of one or more of the following values:
&nbsp;
- **MCL\_CURRENT**
Lock pages that have already been mapped into the address space of the calling process.
&nbsp;
- **MCL\_FUTURE**
Lock pages that has already been mapped and will be mapped in the future into the address space of the calling process. If MCL\_FUTURE is specified, a later call like mmap(), sbrk(), may fail because the number of locked bytes exceeding the maximum.
&nbsp;
The munlockall() function can unlock all virtual address space of the calling process.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

&nbsp;
- **EPERM**
The caller has no permission to do the operation.
&nbsp;
- **ENOMEM**
The locked virtual address space will exceed the RLIMIT\_MEMLOCK limit.
&nbsp;
- **EINVAL**
*flags* is constructed with unknown value.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
