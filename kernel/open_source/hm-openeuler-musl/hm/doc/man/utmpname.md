# utmpname(), utmpxname()
Set the name of the utmp-format file

## Synopsis
```c
#include <utmp.h>

int utmpname(const char *file);
int utmpxname(const char *file);

```

## Arguments
- **file**
The new name of the utmp-format file.

## Library
libc

## Description
The function utmpname() sets the name of the utmp-format file for the other utmp functions to access. The utmpxname() performs the same task. The default path is "/dev/null/utmp" and needs to be reset by the user.

## Returns
- **0**
Set new name successfully.
&nbsp;
- **-1**
Set new name failed.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
