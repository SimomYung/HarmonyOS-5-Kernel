# fputws()
Put a wide-character string on a stream

## Synopsis
```c
#include <wchar.h>

int fputws(const wchar_t *restrict ws, FILE *restrict stream);
```

## Arguments 
- **ws**
A wide-character string will be written.
&nbsp;
- **stream**
The output stream is pointed to by *stream*.

## Library
libc

## Description
The fputws() function writes the string corresponding to the wide-character string pointed to by *ws* to the stream pointed to by *stream*. If there is no character corresponding to the terminating, null wide-character is written.

## Returns
- **A Non-negative Number**
Put the wide-character string successfully, a non-negative number is returned.
&nbsp;
- **-1**
An error occurs, *-1* is returned, an error indicator for the stream is set and *errno* is set.

## Errors
- **EAGAIN**
The *O_NONBLOCK* flag is set for the file descriptor underlying *stream*, and the write operation will be delayed.
&nbsp;
- **EBADF**
The file descriptor underlying *stream* is not valid open for writing.
&nbsp;
- **EFBIG**
Attempt to write to a file that exceeds the maximum file size, or attempt to write to a file that exceeds the file size of the process, or attempt to write to a file at or beyond the offset maximum.
&nbsp;
- **ENOSPC**
There is no free space on the device to store the file.
&nbsp;
- **ENOMEM**
Insufficient memory space is available.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
