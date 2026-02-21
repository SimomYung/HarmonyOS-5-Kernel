# ftok()
Convert a pathname and a project identifier to a System V IPC key

## Synopsis
```c
#include <sys/types.h>
#include <sys/ipc.h>

key_t ftok(const char *path, int id);
```

## Arguments

- **path**
The name of file which is existing and accessible.
&nbsp;
- **id**
A nonzero integer.

## Library
libc

## Description
ftok() uses the identity of the file named by the given *path* (which must refer to an existing, accessible file) and the least significant 8 bits of *id* (which must be nonzero) to generate a key_t type System V IPC key, suitable for use with msgget, semget, or shmget.
&nbsp;
The resulting value is the same for all pathnames that name the same file, when the same value of *id* is used.  The value returned should be different when the (simultaneously existing) files or the *id* differ.

## Returns
- **The generated key_t value**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO
