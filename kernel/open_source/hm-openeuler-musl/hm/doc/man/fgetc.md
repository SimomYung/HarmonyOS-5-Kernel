# fgetc()
Get a byte from stream

## Synopsis
```c
#include <stdio.h>

int fgetc(FILE *stream);
```

## Arguments 
- **stream**
A stream pointer refers to a *FILE* structure.

## Library
libc

## Description
The fgetc() function obtains the next byte as an unsigned char converted to an int from the input stream pointed to by *stream*.

## Returns
Upon successful completion, the next byte from the input stream is returned.

If the end-of-file indicator for the stream is set or the stream is at end-of-file, *EOF* is returned. If an error occurs, the error indicator for the stream is set, *EOF* is returned and *errno* is set.

## Errors
- **EAGAIN**
The *O_NONBLOCK* flag is set for the file descriptor underlying *stream*, the caller will be delayed.
&nbsp;
- **EBADF**
The file descriptor underlying *stream* is not valid for reading.
&nbsp;
- **EOVERFLOW**
Attempt to read at or beyond the offset maximum associated with the corresponding stream.
&nbsp;
- **ENOMEM**
Insufficient memory space is available.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

