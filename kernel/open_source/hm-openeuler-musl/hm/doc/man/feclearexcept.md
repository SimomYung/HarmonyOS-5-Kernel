# feclearexcept()
Clear the exceptions in the floating-point control

## Synopsis
```c
#include <fenv.h>

int feclearexcept(int except);
```

## Arguments
- **except**
Specify the exceptions to be cleared.

## Library
libm

## Description
The function attempts to clear the exceptions selected by *except* in the floating-point status portion of the floating-point control.

## Returns
- **0**
This function always returns 0.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
