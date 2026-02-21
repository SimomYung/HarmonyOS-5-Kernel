# getline()
Read a record line from the stream

## Synopsis
```c
#include <stdio.h>

ssize_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);
```

## Arguments 
- **lineptr**
The argument *lineptr* points to the array of the object.
&nbsp;
- **n**
If the argument *n* is non-zero, the caller reads the object of size at least *n* bytes, or it is a NULL pointer.
&nbsp;
- **stream**
The argument *stream* points to the output stream.

## Library
libc

## Description
The getline() function reads a newline object pointed to by *lineptr* from the *stream*. 

## Returns
When read a record line successfully, The number of characters written into *lineptr* is returned.

If there are no characters are read, the end-of-file for the stream is set, and the stream is at end-of-file, the end-of-file indicator is set and *-1* is returned. 

If an error occurs,  *-1* is returned and the error indicator for the stream is set, the *errno* is set.

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
&nbsp;
- **EINVAL**
The argument *lineptr* or *n* is a NULL pointer.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

