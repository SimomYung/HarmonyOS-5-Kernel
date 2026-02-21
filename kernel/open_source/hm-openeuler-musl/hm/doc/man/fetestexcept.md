# fetestexcept()
Test the exceptions in the floating-point control

## Synopsis
```c
#include <fenv.h>

int fetestexcept(int except);
```

## Arguments
- **except**
Specify the exceptions to be tested.

## Library
libm

## Description
The function tests the exceptions selected by *except* in the floating-point status portion of the floating-point control and returns nonzero if one or more of the exceptions are set.

## Returns
- **zero**
None of the exceptions specified by *except* is set.
&nbsp;
- **nonzero**
One or more of the exceptions are set.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
