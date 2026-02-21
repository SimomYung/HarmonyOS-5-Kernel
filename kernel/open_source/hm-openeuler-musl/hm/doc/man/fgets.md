# fgets()
Get a string from a stream

## Synopsis
```c
#include <stdio.h>

char *fgets(char *restrict s, int n, FILE *restrict stream);
```

## Arguments
- **s**
The bytes which are read from the stream are pointed to by *s*.
&nbsp;
- **n**
*n* - 1 bytes will be read.
&nbsp;
- **stream**
A pointer to the data stream.

## Library
libc

## Description
The fgets() function reads bytes from "stream" into the array pointed to by *s*, until *n* - 1 bytes are read or a newline is read or an end-of-file condition is encountered.

## Returns
- **s**
*s* is returned while getting a string successfully.
&nbsp;
- **null**
If the stream is at end-of-file, a null pointer is returned and the end-of-file indicator is set for the stream. In addition, when a read error occurs, the error indicator for the stream is set and a null pointer is returned.

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
