# fclose()
Close a stream

## Synopsis
```c
#include <stdio.h>

int fclose(FILE *stream);
```

## Arguments 
- **stream**
A stream pointer refers to a *FILE* structure.

## Library
libc

## Description
The fclose() function flushes the stream pointed to by *stream* and closes the file descriptor.

## Returns
- **0**
Close the stream successfully.
&nbsp;
- **EOF**
An error occurs, *EOF* is returned and *errno* is set.

## Errors
- **EBADF**
The file descriptor underlying *stream* is not valid.

The fclose() function may also fail and set *errno* specified for the routines *close*, *write* and *fflush*.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
