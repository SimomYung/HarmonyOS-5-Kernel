# fgetpos()
Get the current file position

## Synopsis
```c
#include <stdio.h>

int fgetpos(FILE *restrict stream, fpos_t *restrict pos);
```

## Arguments 
- **stream**
A stream pointer refers to a *FILE* structure.
&nbsp;
- **pos**
The file position indicator for the stream is stored in *fpos_t* object pointed to by *pos*.

## Library
libc

## Description
The fgetpos() function get the current values of the parse state and file position indicator for the stream pointed to by *stream* in the object pointed to by "pos".

## Returns
- **0**
The error indicator is not set.
&nbsp;
- **non-zero**
The error indicator for *stream* is set.

## Errors
- **EBADF**
The file descriptor underlying *stream* is invalid.
&nbsp;
- **EOVERFLOW**
The value of file position can not be represented correctly in an object of type *fpos_t*.
&nbsp;
- **ESPIPE**
The file descriptor underlying *stream* refers to a pipe, FIFO or socket.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
