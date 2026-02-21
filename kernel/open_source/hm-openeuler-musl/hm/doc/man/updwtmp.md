# updwtmp()
Append an entry to the wtmp file

## Synopsis
```c
#include <utmp.h>

void updwtmp(const char *wtmp_file, const struct utmp *ut);
```

## Arguments
- **wtmp_file**
The file to be updated.
&nbsp;
- **ut**
The entry to be appended.

## Library
libc

## Description
The function updwtmp() appends the utmp structure *ut* to the wtmp file. The default path is "/dev/null/wtmp" and needs to be reset by the user.

## Returns
N/A

## Errors
N/A

## Examples
&nbsp;

## Classification
Unix

## Function Safety
TODO
