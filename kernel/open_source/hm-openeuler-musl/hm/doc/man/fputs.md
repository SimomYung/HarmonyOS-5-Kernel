# fputs()
Put a string on a stream

## Synopsis
```c
#include <stdio.h>

int fputs(const char *restrict s, FILE *restrict stream);
```

## Arguments 
- **s**
The target string is pointed to by *s*.
&nbsp;
- **stream**
The output stream is pointed to by *stream*.

## Library
libc

## Description
The fputs() function writes the null-terminated string pointed to by *s* to the stream pointed to by *stream*.

## Returns
- **A Non-negative Number**
Put a string on the stream successfully a non-negative number is returned. 
&nbsp;
- **EOF**
When an error occurs *EOF* is returned, *errno* is set and an error indicator for the stream is set.

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
- **EPIPE**
Attempt to write to a pipe or FIFO which is not open for reading.
&nbsp;
- **ENOMEM**
Insufficient memory space is available.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO


