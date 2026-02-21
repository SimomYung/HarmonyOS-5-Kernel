# setgid()
Set group ID

## Synopsis
```c
#include <unistd.h>

int setgid(gid_t gid);
```

## Arguments

- **gid**
The group ID to be set.

## Library
libc

## Description
The setgid() function sets the effective group ID of the calling process. The real group ID and saved set-group-ID are also set if the calling process is privileged. This function will not affect the supplementary group list.

## Returns

- **0**
The setgid() function succeeds.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EPERM**
The calling process has no capability(POSIX\_CAP\_SETGID).

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_DAC off, Calling the API under this condition will return 0 and have no effect.
