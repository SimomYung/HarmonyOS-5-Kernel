# pututline()
Writes a utmp structure into the utmp file

## Synopsis
```c
#include <utmp.h>

struct utmp *pututline(struct utmp *ut);
```

## Arguments
- **ut**
The utmp structure to be written into the utmp file.

## Library
libc

## Description
The function pututline() writes the utmp structure ut into the utmp file. If it cannot find an appropriate slot for *ut*, it will append the new entry to the end of the file.

## Return

- **ut**
The function executes successfully.
&nbsp;
- **NULL**
An error occurred.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
