# setfsuid()
Set a user ID

## Synopsis
```c
#include <sys/fsuid.h>

int setfsuid(uid_t uid);
```

## Arguments

- **uid**
The new user id which supposed to be set.

## Library
libc

## Description
Function setfsuid() sets a new user ID to current file(system). Currently, EINVAL is not supported yet.

## Returns

- **Old uid**
Function finished successfully.
&nbsp;
- **uid**
When function failed, the input *uid* will be returned.
&nbsp;

## Errors

- **EPERM**
The caller has no capability(POSIX\_CAP\_SETUID).
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_DAC off, Calling the API under this condition will return 0 and have no effect.
