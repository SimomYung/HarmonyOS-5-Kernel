# fwscanf()
Read the formatted wide-character input

## Synopsis
```c
#include <stdio.h>
#include <wchar.h>

int fwscanf(FILE *restrict stream, const wchar_t *restrict format, ...);
```
## Arguments 
- **stream**
The argument *stream* points to the input stream.
&nbsp;
- **format**
The argument *format* indicates the format of wide-character input.

## Library
libc

## Description
The fwscanf() function reads wide-character in *format* from the input stream pointed to by *stream*.

## Returns
When read the data successfully, the number of successfully matched and assigned input items is returned.

The number can be zero while matching failed. If the input terminates before the first matching failure or conversion, EOF shall be returned. If any error occurs, EOF shall be returned and *errno* is set to indicate the error.  If a read error occurs, the error indicator for the stream is set.

## Errors
- **EAGAIN**
The *O_NONBLOCK * is set for the file descriptor underlying *stream* and the caller will be delayed.
&nbsp;
- **EOVERFLOW**
Attempt to read at or beyond the offset maximum associated with the corresponding stream.
&nbsp;
- **ENOMEM**
Insufficient memory space is available.
&nbsp;
- **EINVAL**
The arguments are insufficient.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
