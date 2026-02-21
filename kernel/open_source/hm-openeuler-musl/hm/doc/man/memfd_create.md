# memfd_create()
Create an anonymous file that supports seal

## Synopsis
```c
#define _GNU_SOURCE
#include <sys/mman.h>

int memfd_create(const char *name, unsigned int flags);
```
## Arguments
- **name**
The name of the anonymous file used for debugging.
&nbsp;
- **flags**
Flags to change the behavior of memfd_create.
&nbsp;

## Library
libc

## Description
memfd_create() creates an anonymous file and returns a file descriptor that refers to it. The file can be modified, truncated, memory-mapped, and so on. The initial size of the file is set to 0, the file size should be set using ftruncate or by writing. The argument *name* is always prefixed with memfd: for debugging purposes. Multiple files can have the same name without any side affects. The argument *flags* must include one of the following values: MFD\_CLOEXEC, MFD\_ALLOW\_SEALING or zero.

- MFD\_CLOEXEC
Set the close-on-exec (FD\_CLOEXEC) flag on the new file descriptor.
- MFD\_ALLOW\_SEALING
Allow sealing operations on this file. If this flag is not set, the initial value of seals will be F_SEAL_SEAL, meaning that no other seals can be set on the file.
- MFD\_HUGETLB
The anonymous file will be created in the hugetlbfs filesystem using huge pages.
- MFD\_HUGE\_2MB, MFD\_HUGE\_4MB, MFD\_HUGE\_1GB
Used in conjunction with MFD\_HUGETLB to select alternative hugetlb page sizes (respectively, 2MB, 4MB, 1GB) on systems that support multiple hugetlb page sizes.
&nbsp;

## Returns
On success, memfd\_create() returns the new file descriptor. On error, -1 is returned and errno is set to indicate the error.

## Errors

- **EFAULT**
The address of *name* points to invalid memory.
&nbsp;
- **EINVAL**
*flags* included unknown bits.
*name* is too long(The limit is 249 bytes, excluding the terminating null byte).
&nbsp;
- **ENOMEM**
There was insufficient memory to create a new anonymous file.
&nbsp;
- **EMFILE**
The per-process limit on the number of open file descriptors has been reached.
&nbsp;
- **EOPNOTSUPP**
The argument *flags* contains flag that is not supported, such as MFD_HUGETLB.
&nbsp;

## Example
&nbsp;

## Classification
Linux-specific

## Function Safety
TODO
