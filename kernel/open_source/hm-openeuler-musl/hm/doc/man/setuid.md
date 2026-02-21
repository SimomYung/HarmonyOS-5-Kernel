# setuid()
Set user ID

## Synopsis
```c
#include <unistd.h>

int setuid(uid_t uid);
```

## Arguments

- **uid**
The user ID to be set.

## Library
libc

## Description

The setuid() function will set the real user ID, effective user ID and the saved set-user-ID of the calling process to *uid* if the calling process has capability(POSIX\_CAP\_SETUID). If not, but *uid* is equal to the real user ID or the saved set-user-ID, the effective user ID will be set to *uid*, with the real user ID and the saved set-user-ID unchanged.
&nbsp;

The supplementary group list will not be changed.

## Returns

- **0**
The setuid() function succeeds.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EPERM**
The calling process does not have capability(POSIX\_CAP\_SETUID) and *uid* is not equal to the real user ID or the saved set-user-ID.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_DAC off, Calling the API under this condition will return 0 and have no effect.
