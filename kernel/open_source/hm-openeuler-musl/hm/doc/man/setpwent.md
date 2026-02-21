# setpwent()
Rewind password database file

## Synopsis
```c
#include <pwd.h>

void setpwent(void);
```
## Arguments
N/A

## Library
libc

## Description
The setpwent() function rewinds to start of password name database file. It's provided for programs that make multiple lookups in password database (using getpwuid() and getpwnam() calls) to avoid opening and closing password database for each access.

## Returns
N/A

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
