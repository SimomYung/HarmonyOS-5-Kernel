# setreuid(), setregid()
Set real and effective user/group IDs

## Synopsis
```c
#include <unistd.h>

int setreuid(uid_t ruid, uid_t euid);
int setregid(gid_t rgid, gid_t egid);
```

## Arguments

- **ruid/rgid**
The real user/group ID to be set.
&nbsp;
- **euid/egid**
The effective user/group ID to be set.

## Library
libc

## Description

The setreuid() and setregid() functions set the real and effective user/group IDs of the calling process. If the argument *ruid/euid* or *rgid/egid* is equal to *-1*, the corresponding user/group ID will not be changed. The arguments *ruid/rgid* and *ruid/egid* can have different values.
&nbsp;

Only a process has capability(POSIX\_CAP\_SETUID or POSIX\_CAP\_SETGID) can set the real and effective user/group ID to any valid value.
&nbsp;

If the real user/group ID or the effective user/group ID to be set is not equal to the real user/group ID, the saved set-user-ID/set-group-ID of the current process will be set to the new effective user/group ID.
&nbsp;

The supplementary group IDs will not be changed.

## Returns

- **0**
The function succeeds.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EPERM**
The calling process does not have capability(POSIX\_CAP\_SETUID or POSIX\_CAP\_SETGID).

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
These APIs are not enabled when system is compiled with CONFIG\_DAC off, Calling these APIs under this condition will return 0 and have no effect.
