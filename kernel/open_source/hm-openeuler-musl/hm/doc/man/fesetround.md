# fesetround()
Set the current rounding mode

## Synopsis
```c
#include <fenv.h>

int fesetround(int mode);
```

## Arguments
- **mode**
The rounding-mode to be set.

## Library
libm

## Description
The function sets the current rounding-mode from *mode* in the floating-point control. An invalid value of *mode* leaves the rounding mode unchanged.

## Returns
- **0**
The rounding mode is successfully set to *mode*.
&nbsp;
- **nonzero**
The *mode* is invalid.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
