# fflush()
Flush a stream

## Synopsis
```c
#include <stdio.h>

int fflush(FILE *stream);
```

## Arguments 
- **stream**
A stream pointer refers to a *FILE* structure.

## Library
libc

## Description
For output stream, fflush() function forces a write of all user-space buffered data for the given output or update stream pointed to by *stream*. For input streams with seekable files, fflush() discards any buffered data that has been fetched from the file, but has not been consumed by caller.

If *stream* is a NULL pointer, it flushes all open output streams, and the open status of the stream is unaffected. 

## Returns
- **0**
Flush the buffered data successfully.
&nbsp;
- **EOF**
An error occurs, *EOF* is returned and *errno* is set.

## Errors
- **EAGAIN**
The *O_NONBLOCK* flag is set for the file descriptor underly *stream*, then the write operation will be delayed.
&nbsp;
- **EBADF**
The file descriptor underlying stream is invalid.
&nbsp;
- **EFBIG**
Attempt to write a file exceeds the maximum file size, the file size limit of the process or beyond the offset maximum associated with the corresponding stream.
&nbsp;
- **ENOMEM**
The stream is created by *open_memstream()* or *open_wmemstream()* and insufficient memory is available.
&nbsp;
- **EPIPE**
Attempt to write a pipe or FIFO which is not open for reading.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
