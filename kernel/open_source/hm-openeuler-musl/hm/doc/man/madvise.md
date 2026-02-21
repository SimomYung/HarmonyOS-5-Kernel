# madvise()
Give advice about use of memory

## Synopsis
```c
#include <sys/mman.h>

int madvise(void *addr, size_t length, int advice);
```

## Arguments

- **addr**
The start address of the memory.
&nbsp;
- **length**
The length of the memory.
&nbsp;
- **advice**
The advice given to the kernel.


## Library
libc

## Description
The madvise() function gives advice to the kernel about the usage of the memory beginning at *addr* and continuing for *length* bytes (*length* is 0, permitted). The argument *advice* can tell the kernel how the memory is expected to be used, so that the kernel can do something to improve the performance. The argument *advice* can be one of the following values:
&nbsp;
- **MADV\_DONTNEED**
The memory will not be accessed in the near future, so the kernel is allowed to free the resources associated with the memory. After a successful MADV\_DONTNEED operation, trying to access the memory will result in page fault. For shared memory, the kernel may not free the pages immediately after the MADV\_DONTNEED operation and the kernel can choose a appropriate moment to free the pages. MADV\_DONTNEED can not be applied to locked pages, Huge TLB pages.
&nbsp;
- **MADV\_NORMAL**
No special operation. This is default state for madvise.
&nbsp;
- **MADV\_RANDOM**
Expect pages loading in random order. It is a clock-wise of MADV\_SEQUENTIAL.
&nbsp;
- **MADV\_SEQUENTIAL**
Expect pages preloading in sequential order. Owing to pages in page cache set as order in kernel, now only permit maximum 16 pages during readahead.
&nbsp;
- **MADV\_WILLNEED**
Expect some pages to be able to use in the future. The number of pages are limited by *length* and the loaded pages in page cache. Once a page exists in page cache, E\_HM\_EXIST may happen.
Currently madvise does not support swapping. MADV\_WILLNEED can not be applied to Huge TLB pages.
&nbsp;
- **MADV\_REMOVE**
Free up shared file mapping pages and its backups. As a result, vfs fallocate is used here to punch holes in the middle of file. MADV\_REMOVE cannot be applied to locked pages, Huge TLB pages.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
The value of *advice* is invalid or unsupported.
&nbsp;
- **EINVAL**
The value of *addr* is not a multiple of the page size.
&nbsp;
- **EINVAL**
The sum of *addr* and *length* exceeds the maximum limit.
&nbsp;
- **EINVAL**
The advice is MADV\_DONTNEED or MADV\_REMOVE and the specified address range includes locked, Huge TLB pages. The advice is MADV\_WILLNEED and the specified address range includes Huge TLB pages.
&nbsp;
- **ENOMEM**
The memory region contains the pages which are unmapped in the address space of the calling process.
&nbsp;
- **EACCES**
Only for advice is MADV\_REMOVE and the virtual address range is not a shared writable mapping.
&nbsp;
- **EBADF**
Only for advice is MADV\_WILLNEED and the virtual memory area is neither normal file nor share memmory.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
