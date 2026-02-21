# mprotect()
Set access protection on a region of memory

## Synopsis
```c
#include <sys/mman.h>

int mprotect(void *addr, size_t len, int prot);
```

## Arguments

- **addr**
The start address of the memory.
&nbsp;
- **len**
The length of the memory.
&nbsp;
- **prot**
The access protection of the memory.

## Library
libc

## Description
The mprotect() function changes the access protection of a memory region of the calling process. The memory region starts at *addr* and continues for *len* bytes. The access protection of all pages which contain a part of the specified memory region will be changed. If the calling process tries to access memory in a manner violating the protection, SIGSEGV signal will be delivered to the process. *addr* must be aligned to page size.
&nbsp;
The argument *prot* can be constructed as the bitwise OR of one or more of the following values:
&nbsp;
- **PROT\_NONE**
The memory can not be accessed.
&nbsp;
- **PROT\_READ**
The memory can be read.
&nbsp;
- **PROT\_WRITE**
The memory can be written.
&nbsp;
- **PROT\_EXEC**
The memory can be executed.
&nbsp;
PROT\_GROWSUP and PROT\_GROWSDOWN are not supported by now.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*addr* is invalid or not aligned to the page size.
&nbsp;
- **EINVAL**
*prot* is constructed with invalid value.
&nbsp;
- **EINVAL**
Both PROT\_GROWSUP and PROT\_GROWSDOWN are specified in *prot*.
&nbsp;
- **ENOSYS**
Either PROT\_GROWSUP or PROT\_GROWSDOWN is specified in *prot*.
&nbsp;
- **ENOMEM**
The number of the mappings with distinct access protection will exceed the maximum.
&nbsp;
- **ENOMEM**
The memory region contains pages which is unmapped in the address space of the calling process.
&nbsp;
- **ENOMEM**
The sum of *addr* and *len* exceeds the maximum limit.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
