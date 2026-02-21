# shm\_unlink()
Remove a shared memory object

## Synopsis
```c
#include <sys/mman.h>

int shm_unlink(const char *name);
```

## Arguments

- **name**
The name of the shared memory object.
&nbsp;

## Library
libc

## Description
Remove the name of the shared memory object named by the string pointed to by *name*. The name is removed before shm\_unlink() returns, but the removal of the memory object is postponed until all open and map references to the shared memory object have been removed.

## Returns

- **0**
The name has been removed.
&nbsp;

- **-1**
An error occurred. The named shared memory object is not be changed by this function call.

## Errors

- **EACCES**
Permission is denied to unlink the named shared memory object.
&nbsp;
- **ENAMETOOLONG**
The length of the name argument is too long.
&nbsp;
- **ENOENT**
The named shared memory object does not exist.
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
