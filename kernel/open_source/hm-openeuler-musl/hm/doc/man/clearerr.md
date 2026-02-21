# clearerr()
Clear the indicators on a stream

## Synopsis
```c
#include <stdio.h>

void clearerr(FILE *stream);
```

## Arguments 
- **stream**
A stream pointer refers to a *FILE* structure.

## Library
libc

## Description
The clearerr() function clears the end-of-file and error indicators for a *FILE* stream pointed by *stream*, it does not change the setting of *errno*.

## Returns
N/A

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO




