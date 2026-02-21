# timegm()
Inverse of gmtime

## Synopsis
```c
#include <time.h>

time_t timegm(struct tm *tm);
```

## Arguments

- **tm**
Pointer to the structure where the broken-down time stores.

## Library
libc

## Description
The timegm() function is the inverse of gmtime(). It takes a broken-down time and convert it to the time in seconds since the Epoch. It takes the input value to be UTC when doing the conversion.

## Returns
Upon successful completion, timegm() returns the time in seconds since the Epoch.
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
