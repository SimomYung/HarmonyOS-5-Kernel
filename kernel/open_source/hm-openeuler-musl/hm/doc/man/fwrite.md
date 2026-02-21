# fwrite()
Write the data to the stream

## Synopsis
```c
#include <stdio.h>

size_t fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);
```

## Arguments 
- **ptr**
The argument *ptr* points to the source data.
&nbsp;
- **size**
The size of each element.
&nbsp;
- **nitems**
The number of elements.
&nbsp;
- **stream**
A pointer refers to the stream for output.

## Library
libc

## Description
The fwrite() function writes from the array pointed by *ptr*, up to *nitems* elements whose size is *size*, to the stream pointed by *stream*. The file-position indicator of the stream shall be advanced by the number of bytes successfully written. If an error occurs, the result value of the file-position indicator is unspecified.

## Returns
When do fwrite() successfully, *nitems* the number of elements written is returned. If *size* or *nitems* is 0, fwrite returns 0 and the state of the stream will not be changed. 

If an error occurs, the number less than *nitems* is returned, the error indicator for the stream is set and *errno* is set.

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
