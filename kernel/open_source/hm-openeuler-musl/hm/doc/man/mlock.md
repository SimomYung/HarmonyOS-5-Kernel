# mlock(), mlock2(), munlock()
Lock and unlock memory of the specified range

## Synopsis
```c
#define _GNU_SOURCE     /* for mlock2 */
#include <sys/mman.h>

int mlock(const void *addr, size_t len);
int mlock2(const void *addr, size_t len, unsigned int flags);
int munlock(const void *addr, size_t len);
```

## Arguments

- **addr**
The start address of the memory.
&nbsp;
- **len**
The length of the memory.
&nbsp;
- **flags**
The flags to control the memory lock operation.


## Library
libc

## Description
The mlock() function locks partial virtual address space of the calling process into RAM, so that the memory will not be swapped out of RAM. The locked virtual address starts at *addr* and continues for *len* bytes. All pages which contain a part of the specified virtual address region will be locked into RAM if the call return successfully.
&nbsp;
The mlock2() function locks partial virtual address space of the calling process into RAM, so that the memory will not be swapped out of RAM. The locked virtual address starts at *addr* and continues for *len* bytes. However, the state of the pages contained in that range after the call returns successfully will depend on the value in the *flags* argument. The *flags* argument can be either 0 or the following constant:
&nbsp;
- **MLOCK\_ONFAULT**
Lock pages that are currently resident and mark the entire range so that the remaining nonresident pages are locked when they are populated by a page fault.
&nbsp;
If flags is 0, mlock2() behaves exactly the same as mlock().
&nbsp;
The munlock() function performs the converse operation, unlocking partial virtual address space of the calling process. All pages which contain a part of the specified virtual address region will be unlocked.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
The sum of *addr* and *len* exceeds the maximum limit.
&nbsp;
- **EINVAL**
Unknown flags were specified for mlock2().
&nbsp;
- **ENOMEM**
The locked region contains pages which is unmapped in the address space of the calling process.
&nbsp;
- **ENOMEM**
The caller has a nonzero RLIMIT\_MEMLOCK soft resource limit and the locked virtual address space exceeds the limit, but the caller has no privilege (**POSIX_CAP_IPC_LOCK**).
&nbsp;
- **ENOMEM**
The number of the mappings with distinct attributes will exceed the maximum.
&nbsp;
- **EPERM**
The caller has no permission to do the operation. The RLIMIT\_MEMLOCK soft resource limit is set to 0, but the caller has no privilege (**POSIX_CAP_IPC_LOCK**).
&nbsp;
- **EAGAIN**
Part of the address space can not be locked.


## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
