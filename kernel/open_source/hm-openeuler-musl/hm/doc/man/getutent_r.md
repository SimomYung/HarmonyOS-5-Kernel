# getutent_r()
Read a line from the current file position in the utmp file

## Synopsis
```c
#include <utmp.h>

int getutent_r(struct utmp *ubuf, struct utmp **ubufp);
```

## Arguments
- **ubuf**
A place to store the line read from the current file position in the utmp file.
&nbsp;
- **ubufp**
A pointer to the result.

## Library
libc

## Description
The getutent_r() function is a GNU extension of getutent(), and it is used to read a line from the current file position in the utmp file.

## Returns
- **0**
Read the line successfully.
&nbsp;
- **-1**
Failure to read the line from the current file position in the utmp file.

## Errors
- **ENOMEM**
Out of memory.
&nbsp;
- **ESRCH**
Record not found.

## Examples
&nbsp;

## Classification
Unix

## Function Safety
TODO
