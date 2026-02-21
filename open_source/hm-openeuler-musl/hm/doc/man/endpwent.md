# endpwent()
Close password database file

## Synopsis
```c
#include <pwd.h>

void endpwent(void);
```
## Arguments
N/A

## Library
libc

## Description
The endpwent() function closes password name database file, so all password access routines behave as if setpwent() had never been called.

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
