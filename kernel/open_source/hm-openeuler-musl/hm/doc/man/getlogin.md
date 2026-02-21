# getlogin()
Return the name of the user who logins.

## Synopsis
```c
#include <unistd.h>

char *getlogin(void);
```

## Arguments

N/A

## Library
libc

## Description
The getlogin() function gets the name by the environment variable named *LOGNAME*. The string is statically allocated, so it should not be freed by callers and it might be invalid when the environment variable is changed.

## Returns

Character pointer to the string containing the user name when successful, or *NULL* if no users login.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
