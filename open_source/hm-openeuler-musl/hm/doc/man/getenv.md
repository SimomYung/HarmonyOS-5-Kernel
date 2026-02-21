# getenv(), secure\_getenv()
Get the corresponding value of the specified environment variable

## Synopsis
```c
#include <stdlib.h>

char *getenv(const char *name);
char *secure_getenv(const char *name);
```

## Arguments

- **name**
A pointer to the environment variable name.

## Library
libc

## Description
The getenv() function searches the environment variables list of the calling process for the environment variable *name* if it exists, and returns a pointer to the value of the environment variable. If the specified environment variable cannot be found, NULL is returned.
&nbsp;
The GNU-specific secure\_getenv() function currently behaves the same as getenv(). It should do some check for user ID and group ID but we do not support now.

## Returns

- **valid string**
The specified environment variable exists.
&nbsp;
- **NULL**
The specified environment variable cannot be found.

## Errors
N/A

## Example
&nbsp;

## Classification
getenv(): POSIX 1003.1-2017
&nbsp;
secure\_getenv(): Unix

## Function Safety
TODO
