# getutline()
Search forward from the current file position in the utmp file

## Synopsis
```c
#include <utmp.h>

struct utmp *getutline(struct utmp *ut);
```

## Arguments
- **ut**
Specified which kind of entry to return.

## Library
libc

## Description
The function getutline() searches forward from the current file position in the utmp file. It scans entries whose ut_type is USER_PROCESS or LOGIN_PROCESS and returns the first one whose ut_line field matches *ut->ut_line*.

## Returns
- **A pointer to the struct utmp**
On success
&nbsp;

- **NULL**
Fails to read a line from the current file position in the utmp based on *ut*.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Functions Safety
TODO
