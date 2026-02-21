# difftime()
Compute the difference between two calendar time values

## Synopsis
```c
#include <time.h>

double difftime(time_t time1, time_t time0);
```

## Arguments

- **time1**
The minuend time value.
&nbsp;
- **time0**
The subtrahend time value.

## Library
libc

## Description
The difftime() function computes the difference between two calendar times: time1 − time0.

## Returns
The difftime() function returns the difference expressed in seconds as a type double.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO