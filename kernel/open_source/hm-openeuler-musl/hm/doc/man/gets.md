# gets()
Get a string from a stdin stream

## Synopsis
```c
#include <stdio.h>

char *gets(char *s);
```

## Arguments 
- **s**
The argument *s* points to the array which stores the string.

## Library
libc

## Description
The gets() function reads bytes from the stdio stream into the array pointed to by *s*, until a newline is read or an end-of-file is encountered.

## Returns
- **s**
Get a string from a stdin stream successfully, *s* is returned.
&nbsp;
- **NULL**
If the end-of-file indicator for the stream is set, or if the stream is at end-of-file, the end-of-file indicator for the stream is set and a NULL pointer is returned. If a read error occurs a NULL pointer is returned, the error indicator for the stream is set and *errno* is set.

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

