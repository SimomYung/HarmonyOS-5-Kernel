# clock()
Determine processor time

## Synopsis
```c
#include <time.h>;

clock_t clock(void);
```

## Arguments
N/A

## Library
libc

## Description
The clock() function returns an approximation of processor time used by the program.

## Returns
The value returned is the CPU time used in units of CLOCKS\_PER\_SEC.
&nbsp;
- **(clock\_t) -1**
The processor time used is not available or its value cannot be represented.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO