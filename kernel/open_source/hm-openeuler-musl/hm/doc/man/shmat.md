# shmat()
System V shared memory operations

## Synopsis
```c
#include <sys/shm.h>

void *shmat(int shmid, const void *shmaddr, int shmflg);
```

## Arguments

- **shmid**
Identify the shared memory.
&nbsp;
- **shmaddr**
Specify the address of shared memory.
&nbsp;
- **shmflg**
Flags that specify the operation.

## Library
libc

## Description
The shmat() attaches the System V shared memory segment identified by *shmid* to the address space of the calling process. The attaching address is specified by *shmaddr* with one of the following criteria:
&nbsp;
* If *shmaddr* is NULL, the system chooses a suitable (unused) page-aligned address to attach the segment.
&nbsp;
* If *shmaddr* isn't NULL and SHM_RND is specified in *shmflg*, the attach occurs at the address equal to *shmaddr* rounded down to the nearest multiple of SHMLBA.
&nbsp;
* Otherwise, *shmaddr* must be a page-aligned address at which the attach occurs.
&nbsp;
In addition to SHM_RND, the following flags may be specified in the *shmflg* bit-mask argument:
&nbsp;
* SHM_RDONLY
Attach the segment for read-only access. The process must have read permission for the segment. If this flag is not specified, the segment is attached for read and write access, and the process must have read and write permission for the segment. There is no notion of a write-only shared memory segment.
&nbsp;
* SHM_EXEC
Allow the contents of the segment to be executed. The caller must have execute permission on the segment.

## Returns

On success, shmat() returns the address of the attached shared memory segment.
&nbsp;
On error, (void *) -1 is returned, and errno is set to indicate the cause of the error.

## Errors

- **EACCES**
The calling process does not have the required permissions for the requested attach type.
&nbsp;
- **EINVAL**
Invalid *shmid* value, unaligned (i.e., not page-aligned and SHM_RND was not specified) or invalid *shmaddr* value, or can't attach segment at *shmaddr*.
&nbsp;
- **ENOMEM**
Could not allocate memory for the descriptor or for the page tables.

## Example
&nbsp;

## Classification
POSIX.1-2001, POSIX.1-2008, SVr4.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
