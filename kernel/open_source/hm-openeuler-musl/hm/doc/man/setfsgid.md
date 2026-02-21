# setfsgid()
Set group identity used for filesystem checks

## Synopsis
```c
#include <sys/fsuid.h>

int setfsgid(gid_t gid);
```
## Arguments
- **gid**
Filesystem group ID that you want to use for process.

## Library
libc

## Description
The setfsgid() changes value of process's filesystem group ID, the group ID that kernel uses to check for all accesses to filesystem. If effective group ID is changed, filesystem group ID will also be changed to new value of effective group ID.
&nbsp;
If process has capability(POSIX\_CAP\_SETGID), the setfsgid() function sets filesystem group ID to *fsgid*.
&nbsp;
If process doesn't have capability(POSIX\_CAP\_SETGID), but *fsgid* is equal to real group ID or saved set-group ID, setfsgid() sets filesystem group ID to *fsgid*.

## Returns
On both success and failure, this function returns previous filesystem group ID of process.

&nbsp;
## Errors
N/A

&nbsp;
## Examples
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_DAC off, Calling the API under this condition will return 0 and have no effect.
