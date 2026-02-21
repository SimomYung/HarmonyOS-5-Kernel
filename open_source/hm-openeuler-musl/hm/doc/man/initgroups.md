# initgroups()
Initialize supplementary group access list

## Synopsis
```c
#include <sys/types.h>
#include <grp.h>

int initgroups(const char *user, gid_t gid);
```
## Arguments
- **user**
The name of user whose group membership you want to use as supplementary group access list.
&nbsp;
- **gid**
A group ID that you want to include in the group access list.

## Library
libc

## Description
The initgroups() function reads group membership for user specified by name from group database, and then initializes supplementary group access list of calling process.
&nbsp;
If number of groups in the supplementary access list exceeds NGROUPS\_MAX, extra groups are ignored.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **ENOMEM**
Insufficient memory to allocate group information structure.
&nbsp;
- **EPERM**
The calling process has insufficient privilege.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
