# setresuid(), setresgid()


## Synopsis
```c
#define _GNU_SOURCE
#include <unistd.h>

int setresuid(uid_t ruid, uid_t euid, uid_t suid);
int setresgid(gid_t rgid, gid_t egid, gid_t sgid);
```

## Arguments

- **ruid/rgid**
The real user/group ID to be set.
&nbsp;
- **euid/egid**
The effective user/group ID to be set.
&nbsp;
- **suid/sgid**
The saved user/group ID to be set.

## Library
libc

## Description

The setresuid() and setresgid() functions set the real, the effective and the saved user/group ID of the calling process. If the calling process has no capability(POSIX\_CAP\_SETUID or POSIX\_CAP\_SETGID), it can only change its IDs to the current ones. Arbitrary values can be set to if the process has the capability.
&nbsp;

If one of the arguments is equal to *-1*, the corresponding ID will not be changed.
&nbsp;

No matter what changes has been made to the real, the effective and the saved user/group ID, the filesystem user/group ID is always set to the same value as the effective user/group ID.

## Returns

- **0**
The function succeeds.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EPERM**
The calling process tries to change the IDs without capability(POSIX\_CAP\_SETUID or POSIX\_CAP\_SETGID).

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO

## NOTES
These APIs are not enabled when system is compiled with CONFIG\_DAC off, Calling these APIs under this condition will return 0 and have no effect.
