# shmget()
System V shared memory operations

## Synopsis
```c
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflg);
```

## Arguments

- **key**
Key value that used to create or open a System V shared memory.
&nbsp;
- **size**
The size of shared memory.
&nbsp;
- **shmflg**
Flags that specify the operation.

## Library
libc

## Description
The shmget() returns the identifier of the System V shared memory segment associated with the value of the argument *key*. It may be used either to obtain the identifier of a previously created shared memory segment (when *shmflg* is zero and *key* does not have the value IPC_PRIVATE), or to create a new set.
&nbsp;
A new shared memory segment, with size equal to the value of *size* rounded up to a multiple of PAGE_SIZE, is created if *key* has the value IPC_PRIVATE or *key* isn't IPC_PRIVATE, no shared memory segment corresponding to *key* exists, and IPC_CREAT is specified in *shmflg*.
&nbsp;
If *shmflg* specifies both IPC_CREAT and IPC_EXCL and a shared memory segment already exists for *key*, then shmget() fails with errno set to EEXIST.
&nbsp;
The value *shmflg* is composed of:

* IPC_CREAT
Create a new segment. If this flag is not used, then shmget() will find the segment associated with *key* and check to see if the user has permission to access the segment.
&nbsp;
* IPC_EXCL
This flag is used with IPC_CREAT to ensure that this call creates the segment. If the segment already exists, the call fails.
&nbsp;
* SHM_HUGETLB
Using hugepages
&nbsp;
* SHM_NORESERVE
Do not reserve hugepages for this segment.

## Returns

On success, a valid shared memory identifier is returned.
&nbsp;
On error, -1 is returned, and *errno* is set to indicate the error.

## Errors

- **EACCES**
The user does not have permission to access the shared memory segment.
&nbsp;
- **EEXIST**
IPC_CREAT and IPC_EXCL were specified in *shmflg*, but a shared memory segment already exists for *key*.
&nbsp;
- **EINVAL**
A new segment was to be created and size is less than SHMMIN or greater than SHMMAX.
&nbsp;
- **EINVAL**
A segment for the given *key* exists, but *size* is greater than the size of that segment.
&nbsp;
- **ENOENT**
No segment exists for the given *key*, and IPC_CREAT was not specified.
&nbsp;
- **ENOMEM**
No memory could be allocated for segment overhead.
&nbsp;
- **ENOSPC**
All possible shared memory IDs have been taken (SHMMNI), or allocating a segment of the requested *size* would cause the system to exceed the system-wide limit on shared memory (SHMALL).

## Example
&nbsp;

## Classification
POSIX.1-2001, POSIX.1-2008, SVr4.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
