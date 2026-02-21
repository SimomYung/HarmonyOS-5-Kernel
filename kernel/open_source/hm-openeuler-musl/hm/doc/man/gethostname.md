# gethostname()
Get name of current host

## Synopsis
```c
#include <unistd.h>

int gethostname(char *name, size_t namelen);
```

## Arguments

- **name**
The pointer that points to the array that stores the current host name.
&nbsp;
- **namelen**
The size of the array that *name* points to.

## Library
libc

## Description

The gethostname() function saves the standard host name of the current machine in the array that *name* points to. The *namelen* argument specifies the size of the array. If the *namelen* argument is sufficient to save the host name, the *name* will be NULL-terminated. Or it will be unspecified whether the returned name is NULL-terminated.

Host names are limited to {HOST\_NAME\_MAX} bytes.

## Returns

- **0**
The gethostname() function succeeds.
&nbsp;

- **-1**
An error occurred.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
