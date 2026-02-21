# fputc()
Put a byte on a stream

## Synopsis
```c
#include <stdio.h>

int fputc(int c, FILE *stream);
```

## Arguments 
- **c**
The target byte is specified by *c*, which is converted to an unsigned char. 
&nbsp;
- **stream**
The output stream is pointed to by *stream*.

## Library
libc

## Description
The fputc() function writes the byte specified by *c* which is converted to an unsigned char, to the output stream pointed to by *stream* at the position indicated by the associated file-position indicator for the stream (if defined), and shall advance the indicator appropriately. If the file cannot support positioning requests, or if the stream was opened with append mode, the byte shall be appended to the output stream.

## Returns
- **The value has been written**
Write the bytes successfully, the value has been written is returned.
&nbsp;
- **EOF**
An error occurs, *EOF* is returned, the error indicator for the stream is set and *errno* is set. 

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
