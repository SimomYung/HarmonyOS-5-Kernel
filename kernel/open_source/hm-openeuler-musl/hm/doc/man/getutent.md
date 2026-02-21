# getutent()
Read a line from the current file position in the utmp file

## Synopsis
```c
#include <utmp.h>

struct utmp *getutent(void);
```

## Arguments
N/A

## Library
libc

## Description
The getutent() function reads a line from the current file position in the utmp file.

## Returns
- **A pointer to a struct utmp**
The function getutent() successfully reads a line from the utmp file.
&nbsp;
- **NULL**
Fails to read a line from the current file position in the utmp file.

## Errors
- **ENOMEM**
Out of memory.
&nbsp;
- **ESRCH**
Record not found.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
