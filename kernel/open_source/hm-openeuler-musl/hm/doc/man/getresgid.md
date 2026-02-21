# getresgid(), getresuid()
Obtain the group/user ID(s)

## Synopsis
```c
#include <unistd.h>

int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);
int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
```

## Arguments

- **rgid**
The *rgid* will store the returned real group ID.
&nbsp;
- **egid**
The *egid* will store the returned effective group ID.
&nbsp;
- **sgid**
The *sgid* will store the returned saved group ID.
&nbsp;
- **ruid**
The *ruid* will store the returned real user ID.
&nbsp;
- **euid**
The *euid* will store the returned effective user ID.
&nbsp;
- **suid**
The *suid* will store the returned saved user ID.


## Library
libc

## Description
Function getresgid() or getresuid() obtains the group ID(s) or user ID(s) of the system.

## Returns

- **0**
Functions finished successfully.
&nbsp;
- **-1**
Failed to conduct the function. The *errno* code indicates the specific error.
&nbsp;

## Errors

- **EFAULT**
The input parameters are invalid (unlikely to happen).
&nbsp;

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
These APIs are not enabled when system is compiled with CONFIG\_DAC off, Calling these APIs under this condition will return 0 and have no effect.
