# setenv()
Add or change environment variable

## Synopsis
```c
#include <stdlib.h>

int setenv(const char *envname, const char *envval, int overwrite);
```

## Arguments
- **envname**
Pointer to a string containing the name of an environment variable to be added or altered.
&nbsp;
- **envval**
Pointer to the value that shall be set to the environment variable.
&nbsp;
- **overwrite**
Indicate the environment variable shall be updated or unchanged.

## Library
libc

## Description
The setenv() function updates or adds a variable in the environment of the calling process. The environment variable *envname* shall be set to the value to which the *envval* points. If the *overwrite* argument is non-zero, the environment shall be updated, otherwise, the environment shall remain unchanged.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINVAL**
The *envname* is NULL, points to a string of length 0 or containing an '=' character.
&nbsp;
- **ENOMEM**
Insufficient memory to add a new variable to the environment.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO