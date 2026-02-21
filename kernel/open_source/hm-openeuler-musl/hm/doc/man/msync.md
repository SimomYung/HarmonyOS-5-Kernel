# msync()
Synchronize memory with physical storage
## Synopsis
```c
#include <sys/mman.h>

int msync(void *addr, size_t len, int flags);
```

## Arguments

- **addr**
The start address of the memory.
&nbsp;
- **len**
The length of the memory.
&nbsp;
- **flags**
The flags given to msync.


## Library
libc

## Description
The msync() function writes all modified data to permanent storage locations, if any, in those whole pages containing any part of the address space of the process starting at address *addr* and continuing for *len* bytes. If no such storage exists, msync() need not have any effect. If requested, the msync() function then invalidates cached copies of data.
&nbsp;
The implementation requires that *addr* be a multiple of the page size.
&nbsp;
- **MS\_SYNC**
Perform synchronous writes.
When MS\_ASYNC is specified, msync() returns immediately once all the write operations are initiated or queued for servicing; when MS\_SYNC is specified, msync() does not return until all write operations are completed as defined for synchronized I/O data integrity completion.
&nbsp;
- **MS\_ASYNC**
Perform asynchronous writes.
It's not allowed to specify both MS\_ASYNC and MS\_SYNC
&nbsp;
- **MS\_INVALIDATE**
Asks to invalidate other mappings of the same file (so that they can be updated with the fresh values just written).
## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
&nbsp;
- **EBUSY**
Some or all of the addresses in the range starting at *addr* and continuing for *len* bytes are locked, and MS\_INVALIDATE is specified.
&nbsp;
- **EINVAL**
The value of *flags* is invalid or unsupported.
&nbsp;
- **EINVAL**
The value of *addr* is not a multiple of the page size {PAGESIZE}.
&nbsp;
- **EINVAL**
The range of *addr* and *length* exceeds the maximum limit.
&nbsp;
- **ENOMEM**
The memory region was not mapped in the address space of the calling process.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
