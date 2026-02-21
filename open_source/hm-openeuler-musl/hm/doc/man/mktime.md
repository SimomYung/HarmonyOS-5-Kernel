# mktime()
Convert the broken-down time into the time since the Epoch

## Synopsis
```c
#include <time.h>;

time_t mktime(struct tm *timeptr);
```

## Arguments
- **timeptr**
Points to the structure where the broken-down time stores.

## Library
libc

## Description
The mktime() function converts the broken-down time expressed as local time into a time since the Epoch value.

## Returns
Upon successful completion, mktime() returns the time in seconds since the Epoch.
&nbsp;
- **(clock\_t) -1**
An error is encountered.

## Errors
- **EOVERFLOW**
The result cannot be represented.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO