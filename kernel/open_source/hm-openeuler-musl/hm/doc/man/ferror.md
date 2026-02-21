# ferror()
Test the error indicator for a stream

## Synopsis
```c
#include <stdio.h>

int ferror(FILE *stream);
```

## Arguments
- **stream**
A stream pointer refers to a *FILE* structure.

## Library
libc

## Description
The ferror() function tests if the error indicator for the stream pointed to by *stream* is set, it does not change the setting of *errno*.

## Returns
- **0**
The error indicator is not set.
&nbsp;
- **non-zero**
The error indicator for *stream* is set.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
