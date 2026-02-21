# clearenv()
Clear all environment variables

## Synopsis
```c
#include <stdlib.h>

int clearenv(void);
```

## Arguments
N/A

## Library
libc

## Description
The clearenv() function clears all environment variables and sets the value of the external variable *environ* to NULL.

## Returns

- **0**
This function always succeeds.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
