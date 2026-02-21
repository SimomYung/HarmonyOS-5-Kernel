# endgrent()
Close group database file

## Synopsis
```c
#include <grp.h>

void endgrent(void);
```
## Arguments
N/A

## Library
libc

## Description
The endgrent() routine closes group name database file, so all group access routines behave as if setgrent() had never been called.

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
