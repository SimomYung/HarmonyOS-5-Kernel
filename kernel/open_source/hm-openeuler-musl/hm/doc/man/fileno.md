# fileno()
Map a file stream pointer to a file descriptor

## Synopsis
```c
#include <stdio.h>

int fileno(FILE *stream);
```

## Arguments
- **stream**
A stream pointer refers to a *FILE* structure.

## Library
libc

## Description
The fileno() function gets the integer file descriptor associated with the stream pointed to by *stream*.

## Returns
- **An integer value**
Get the file descriptor associated with the stream successfully.
&nbsp;
- **-1**
An error occurs, -1 is returned and the *errno* is set.

## Errors
- **EBADF**
The argument *stream* is invalid or the stream is not associated with a file.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
