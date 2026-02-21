# fread()
Read the binary data

## Synopsis
```c
#include <stdio.h>

size_t fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
```

## Arguments 
- **ptr**
The argument *ptr* points to the buffer which is used to store the data.
&nbsp;
- **size**
The size of each data element.
&nbsp;
- **nitems**
The number of data elements.
&nbsp;
- **stream**
The argument *stream* points to the input stream.

## Library
libc

## Description
The fread() function reads the data from the stream pointed to by *stream* to the array pointed to by *ptr* up to *nitems* elements whose size is specified by *size*. The file position indicator for the stream (if defined) shall be advanced by the number of bytes successfully read. If an error occurs, the resulting value of the file position indicator for the stream is unspecified. If a partial element is read, its value is unspecified.

## Returns
Read the data successfully, *nitems* is returned.
&nbsp;
When an error or end-of-file is encountered, a value less than *nitems* is returned. If *size* or *nitems* is 0, 0 is returned and the state of the array, the state of the stream will not change. If an error occurs, the error indicator for the stream is set and the *errno* is set.

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
