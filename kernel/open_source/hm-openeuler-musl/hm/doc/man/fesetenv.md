# fesetenv()
Set the current floating-point environment

## Synopsis
```c
#include <fenv.h>

int fesetenv(const fenv_t *envp);
```

## Arguments
- **envp**
The floating-point environment to be set.

## Library
libm

## Description
The fesetenv() function restores the floating-point environment from the object pointed to by *envp*. This object must be known to be valid, for example, the result of a call to fegetenv() or feholdexcept() or equal to FE_DFL_ENV.

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
