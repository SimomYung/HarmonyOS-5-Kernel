# mincore()

Indicate whether the virtual memory region is backed by physical pages

## Synopsis

```c
#include <unistd.h>
#include <sys/mman.h>

int mincore(void *addr, size_t length, unsigned char *vec);
```

## Arguments

- **addr**
    The start of the virtual memory region to do mincore, this addr must be pagesize aligned.
    &nbsp;
- **length**
    The length of the virtual memory region.
    &nbsp;
- **vec**
    The vector to store the result of mincore (containing at least (length + PAGE_SIZE - 1) / PAGE_SIZE bytes).
    &nbsp;

## Library

libc

## Description

mincore() returns a vector that indicates whether the calling process's virtual memory region is backed by physical pages, and so will not cause a disk access if referenced.

The addr argument must be a multiple of the system page size. The length argument need not be a multiple of the page size, but since mapping information is returned for whole pages, length is effectively rounded up to the next multiple of the page size. On return, the least significant bit of each byte will be set if the virtual memory region is backed by physical pages, and be clear otherwise.

## Returns

On success, mincore() returns zero..
&nbsp;
On error, mincore() returns -1, and errno is set to indicate the error.

## Errors

- **EAGAIN**
    OS is temporarily out of resources.
    &nbsp;
- **EFAULT**
    vec points to an invalid address.
    &nbsp;
- **EINVAL**
    addr is not a multiple of the page size.
    &nbsp;
- **ENOMEM**
    addr to addr + length contained unmapped memory.

## Example

&nbsp;

## Classification

Linux

## Function Safety

TODO
