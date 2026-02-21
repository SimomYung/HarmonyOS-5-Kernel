# mremap()
Remap a virtual space region

## Synopsis
```c
#include <sys/mman.h>

void *mremap(void *old_addr, size_t old_size,
             size_t new_size, int flags, ... /* void *new_addr */);
```

## Arguments 

- **old_addr**
The start of the virtual memory region to be remap, this addr must be pagesize aligned.
&nbsp;
- **old_size**
The size of the old virtual memory region.
&nbsp;
- **new_size**
The size which old region will be remap to.
&nbsp;
- **flags**
Flags control the remap operation.
&nbsp;
- **new_addr(optional)**
When *flags* specifys *MREMAP_FIXED* then new_addr must be valid to specify where the old region will be remap to.


## Library
libc

## Description
mremap() expands (or shrinks) an existing memory region, potentially moving it at the same time (controlled by the flags argument and the available virtual address space).

*old_addr* is the old address of the virtual memory region that you want to expand (or shrink). Note that *old_addr* has to be page aligned. *old_size* is the old size of the virtual memory region. *new_size* is the requested size of the virtual memory block after the resize. An optional fifth argument, *new_addr*, may be provided if *flags* contains MREMAP_FIXED.

The flags bit-mask argument may be 0, or include the following flag:

- MREMAP_MAYMOVE:
By default, if there is not sufficient space to expand a mapping at its current location, then mremap() fails. If this flag is specified, then the kernel is permitted to relocate the mapping to a new virtual address, if necessary. If the mapping is relocated, then absolute pointers into the old mapping location become invalid (offsets relative to the starting address of the mapping should be employed).

- MREMAP_FIXED:
This flag serves a similar purpose to the *MAP_FIXED* flag of mmap(). If this flag is specified, then mremap() accepts a fifth argument, void *new_addr, which specifies a page-aligned address to which the mapping must be moved. Any previous mapping at the address range specified by new_address and new_size is unmapped. If MREMAP_FIXED is specified, then MREMAP_MAYMOVE must also be specified.

If the memory segment specified by *old_addr* and *old_size* is locked (using *mlock(2)* or similar), then this lock is maintained when the segment is resized and/or relocated. As a consequence, the amount of memory locked by the process may change.

## Returns

On success, the function return new addr.
&nbsp;

On failure, the function will return *MAP_FAILED*.

## Errors

- **EAGAIN**
The old region is mlocked and the new size is exceeds RLIMIT_MEMLOCK
&nbsp;
- **EFAULT**
The old region was invalid.
&nbsp;
- **EINVAL**
The arguments check failed.
&nbsp;
- **ENOMEM**
Mem alloc failed during remap.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
