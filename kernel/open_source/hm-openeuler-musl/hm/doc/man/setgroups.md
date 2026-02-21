# setgroups()
Set a list of available group IDs

## Synopsis
```c
#include <unistd.h>

int setgroups(size_t count, const gid_t list[]);
```

## Arguments

- **count**
The number of gid\_t in the group IDs list.
&nbsp;
- **list**
The buffer where store all the provided available group IDs.

## Library
libc

## Description
Function setgroups() provides a list of available group IDs. Currently, ESRCH is returned when vspace can not be acquired. ENOMEM is not supported yet. Other return error except EINVAL, ESRCH, EPERM may returned.

## Returns

- **0**
Function finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.


## Errors

- **ESRCH**
The vspace is NULL.
&nbsp;
- **EFAULT**
*list* address is invalid.
&nbsp;


## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_DAC off, Calling the API under this condition will return 0 and have no effect.
