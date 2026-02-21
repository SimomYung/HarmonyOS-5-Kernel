# getc()
Get a byte from a stream

## Synopsis
```c
#include <stdio.h>

int getc(FILE *stream);
```

## Arguments 
- **stream**
The argument *stream* points to the output stream.

## Library
libc

## Description
The getc() function obtains the next byte as an unsigned char converted to an int from the input stream pointed to by *stream*.

## Returns
Get a byte successfully, the next byte from the input stream is returned.

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
