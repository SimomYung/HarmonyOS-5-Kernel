# getutid()
Search forward from the current file position in the utmp file

## Synopsis
```c
#include <utmp.h>

struct utmp *getutid(struct utmp *ut);
```

## Arguments
- **ut**
Specified which kind of entry to be return.

## Library
libc

## Description
The getutid() function searches forward from the current file position in the utmp file. If *ut->ut_type* is one of RUN\_LVL, BOOT\_TIME, NEW\_TIME, or OLD\_TIME, getutid() will find the first entry whose ut\_type field matches *ut->ut_type*. If *ut->ut_type* is one of INIT\_PROCESS, LOGIN\_PROCESS, USER\_PROCESS, or DEAD\_PROCESS, getutid() will find the first entry whose ut\_id field matches *ut->ut_id*.

## Returns
- **A pointer to a struct utmp**
Success.
&nbsp;
- **NULL**
Fails to read a line from the current file position in the utmp file based on *ut*.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
