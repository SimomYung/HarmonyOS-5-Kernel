# sem\_unlink()
Remove a named semaphore

## Synopsis
```c
#include <semaphore.h>

int sem_unlink(const char *name);
```

## Arguments

- **name**
The name of the semaphore.

## Library
libc

## Description
The sem\_unlink() function removes the named semaphore. The name referred by *name* will be invalid and the semaphore will be destroyed once all other processes which have opened the semaphore close it.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **ENAMETOOLONG**
The length of *name* exceeds the maximum limit.
&nbsp;
- **ENOENT**
The named semaphore does not exists.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
