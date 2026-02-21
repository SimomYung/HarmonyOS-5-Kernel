# munmap()
Unmap memory

## Synopsis
```c
#include <sys/mman.h>

int munmap(void *addr, size_t length);
```

## Arguments

- **addr**
The path to the file or directory that you want to access.
&nbsp;
- **length**
The length of the new mapping.

## Library
libc

## Description
The munmap() function deletes a mapping in the virtual address space of the calling process. The mapping starts at *addr* and continues for *length* bytes. All pages which contain a part of the specified virtual address region will be unmapped, and SIGSEGV will be generated if try to access the unmapped region. No error will be returned if the specified region does not contain any mapped pages. *addr* must be a multiple of the page size.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*length* is zero.
&nbsp;
- **EINVAL**
*addr* does not meet the alignment requirement.
&nbsp;
- **EINVAL**
*addr* or *length* or the sum of *addr* and *length* exceeds the maximum limit.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
