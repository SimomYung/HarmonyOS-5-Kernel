# unsetenv()
Remove an environment variable

## Synopsis
```c
#include <stdlib.h>

int unsetenv(const char *name);
```

## Arguments
- **name**
Pointer to a string which is the name of the environment variable to be removed.

## Library
libc

## Description
The unsetenv() function removes an environment variable from the environment of the calling process. If the named variable does not exist in the current environment, the environment shall be unchanged and the function is considered to have completed successfully.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINVAL**
The *envname* is NULL, points to a string of length 0 or containing an '=' character.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO