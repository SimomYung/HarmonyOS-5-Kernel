# fegetenv()
Get the current floating-point environment

## Synopsis
```c
#include <fenv.h>

int fegetenv(fenv_t *envp);
```

## Arguments
- **envp**
Store the current floating-point environment.

## Library
libm

## Description
The function attempts to store the settings in the floating-point control at *envp*.

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
