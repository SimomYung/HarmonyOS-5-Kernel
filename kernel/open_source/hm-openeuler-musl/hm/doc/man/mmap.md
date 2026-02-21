# mmap(), mmap64()
Map files or devices into memory

## Synopsis
```c
#include <sys/mman.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
void *mmap64(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
```

## Arguments

- **addr**
The hint to the kernel about where to place the new mapping.
&nbsp;
- **length**
The length of the new mapping.
&nbsp;
- **prot**
The access protection of the new mapping.
&nbsp;
- **flags**
The flags to control the mapping operation.
&nbsp;
- **fd**
The file descriptor of the file mapped into the memory.
&nbsp;
- **offset**
The offset of the file mapped into the memory.

## Library
libc

## Description
The mmap() function creates a new mapping in the virtual address space of the calling process and return the start address of the new mapping. The argument *length* specifies the length of the new mapping. The argument *addr* is a hint to the kernel about the start address of the new mapping. If *addr* is NULL or the mapping according to the hint is inappropriate (like overlapped with another existing mapping), the kernel will choose a new page-aligned address as the start of the new mapping.
Notes: Following operation is not allowed - creates a new mapping and truncates the mapping file smaller when there is no ummaping. Accessing the new mapping may encounter unexpected errors.
&nbsp;
The contents of a file mapping starts at *offset* in the file referred to by *fd* and continues for *length* bytes. *offset* must be a multiple of the page size. The file should be mapped in multiples of the page size. If the mapped size of the file is not a multiple of the page size, the remaining part of the mapping will be filled with zero and changes to that part will not be carried through the underlying file.
&nbsp;
The argument *prot* specifies the access protection of the new mapping. It can be either PROT\_NONE or constructed as the bitwise OR of one or more of the following values:
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

The argument *flags* determines whether the mapping is shared by all processes that have mapped the same region, and whether the changes to the mapping will be carried through the underlying file. So one of the following values must be specified in *flags*.
&nbsp;
- **MAP\_SHARED**
The mapping is shared by all processes that have mapped the same region, and changes to the mapping will be carried through to the underlying file.
&nbsp;
- **MAP\_PRIVATE**
The mapping is copy-on-write and private by the calling process, and changes to the mapping will not be carried through to the underlying file. It is unspecified whether changes to the file after the mmap() call are visible in the mapping.
&nbsp;
In additional, zero or more following values can be ORed in *flags*:
&nbsp;
- **MAP\_ANONYMOUS**
The mapping is not mapped with any file. The contents of the mapping are initialized to zero and *fd* is ignored.
&nbsp;
- **MAP\_ANON**
The same as MAP\_ANONYMOUS.
&nbsp;
- **MAP\_FIXED**
The kernel does not treat *addr* as a hint and the mapping will be placed at the address specified by *addr* exactly. If the mapping specified by *addr* and *length* is overlapped with other existing mapping, the overlapped part of the existing mapping will be discarded. *addr* must be page-aligned. *addr* must be higher than static config **CONFIG_MEM_VM_MIN_ADDR** for low address protection. The default value of this config is 64kb.
&nbsp;
If MAP\_FIXED is set, a failed mmap may free existing mapped virtual address ranges. Because a mmap with MAP\_FIXED unmaps overlapping mapped ranges and then does the actual mapping, depending on whether the failure happens before or after the unmap process, overlapping mapped ranges may or may not be retained after the mmap.
&nbsp;
- **MAP\_FIXED\_NOREPLACE**
This flag has not been supported yet.
&nbsp;
- **MAP\_POPULATE**
The mapping provides page tables and allocates page frames by pre-reading, especially for the file map. As a result, access to the map area will not be blocked by illegal pages.
&nbsp;
- **MAP\_LOCKED**
The mapping is used to populate pages immediately and lock pages in specific areas. The size of locked area is limited by *rlimit*.
&nbsp;
- **MAP\_HUGETLB**
The mapping will use "huge pages".
&nbsp;
- **MAP\_HUGE\_2MB, MAP\_HUGE\_1GB**
Those flags should be used in conjunction with MAP\_HUGETLB to choose page size of "huge page" (like 2 MB and 1 GB). The huge page size can be configured in the "flags" at the offset MAP\_HUGE\_SHIFT by the base-2 logarithm form. For example, the above flags are defined as:

    \#define MAP\_HUGE\_2MB   (21 << MAP_HUGE_SHIFT)
    \#define MAP\_HUGE\_1GB   (30 << MAP_HUGE_SHIFT)

The mmap64() function is equivalent to mmap().
&nbsp;
- **MAP\_NORESERVE**
The mapping will not reserve physical memory, the allocation of physical memory is done during page fault. If MAP\_NORESERVE is not set, the mapping will reserve adequate physical memory for preparation.
&nbsp;
- **MAP\_STACK**
Allocate the mapping at an address suitable for a process or thread stack.
&nbsp;
- **MAP\_GAP**
The mapping tries to further randomize the mmap address by inserting random gaps after the front mapped regions. But this flag will be ignored if the feature is turned off by kernel. In addition, the flag will be ignored when the mapped address has been specified or MAP\_FIXED/MAP\_HUGETLB flag is enabled.
&nbsp;
- **MAP\_XPM**
This flag is used to map ABC code into the specific memory range named `xpm region`. It relies on enabling XPM feature.
&nbsp;

## Returns

- **pointer to the mapped area**
If the call succeeds, the pointer to the mapped area will be returned.
&nbsp;
- **MAP\_FAILED**
An error occurred.

## Errors

- **EACCES**
A file mapping is requested but *fd* is not open for reading. Or MAP\_SHARED is requested and PROT\_WRITE is set but *fd* is not open in O\_RDWR mode. Or PROT\_WRITE is set but the file is append-only.
&nbsp;
- **EBADF**
Neither MAP\_ANONYMOUS or MAP\_ANON is contained in *flags* and *fd* is an invalid file descriptor.
&nbsp;
- **EINVAL**
None of MAP\_PRIVATE or MAP\_SHARED is contained in *flags*. Or both MAP\_PRIVATE and MAP\_SHARED are contained in *flags*.
&nbsp;
- **EINVAL**
*length* is zero.
&nbsp;
- **EINVAL**
*addr* or *offset* does not meet the alignment requirement.
&nbsp;
- **EINVAL**
*addr* or *length* or the sum of *addr* and *length* exceeds the maximum limit.
&nbsp;
- **ENFILE**
The number of open file descriptors exceeds the system-wide limit. The max number of file mappings that one process can create is limited to 512 due to current implementation. If file mappings exceed limitation, ENFILE will be returned.
&nbsp;
- **ENODEV**
The filesystem of the underlying file does not support memory mapping.
&nbsp;
- **ENOMEM**
There is insufficient memory to fulfill the mapping request.
&nbsp;
- **ENOMEM**
The mapped address space will exceed the RLIMIT\_AS or RLIMIT\_DATA limit.
&nbsp;
- **EPERM**
The caller has no permission to do the operation. *flags* is set to MAP\_LOCKED | MAP\_FIXED and the RLIMIT\_MEMLOCK soft resource limit is set to 0, but the caller has no privilege (**POSIX_CAP_IPC_LOCK**).
&nbsp;
- **EAGAIN**
*flags* is set to MAP\_LOCKED, the caller has a nonzero RLIMIT\_MEMLOCK soft resource limit and *length* exceeds the limit, but the caller has no privilege (**POSIX_CAP_IPC_LOCK**).

&nbsp;
## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
