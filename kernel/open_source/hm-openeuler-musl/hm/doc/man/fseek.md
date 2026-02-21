# fseek()
Reposition a file-position indicator in a stream

## Synopsis
```c
#include <stdio.h>

int fseek(FILE *stream, long offset, int whence);
```

## Arguments 
- **stream**
The target I/O *stream*.
&nbsp;
- **offset**
The value of *offset* that the file-position indicator shall move.
&nbsp;
- **whence**
It indicates the location of the file-position indicator at present.

## Library
libc

## Description
The *fseek()* function sets the file-position indicator for the stream pointed to by *stream*. When an I/O error occurs, the error indicator for the stream is set and it fails.

## Returns
- **0**
Reposition the file-position indicator successfully.
&nbsp;
- **-1**
An error occurs, -1 is returned and *errno* is set.

## Errors
- **EAGAIN**
The O_NONBLOCK flag is set for the file descriptor and the thread would be delayed in the write operation.
&nbsp;
- **EBADF**
The file descriptor underlying the stream file is not open for writing or the stream's buffer needed to be flushed and the file is not open.
&nbsp;
- **EFBIG**
Attempt to write to a file that exceeds the maximum file size, or attempt to write to a file that exceeds the file size of the process, or attempt to write to a file at or beyond the offset maximum.
&nbsp;
- **EINVAL**
The argument *whence* is invalid and file-position indicator will be set to a negative value.
&nbsp;
- **ENOSPC**
There is no free space on the device to store the file.
&nbsp;
- **EOVERFLOW**
The value of file offset can not be represent correctly in an object in *off_t* type.
&nbsp;
- **ESPIPE**
The file descriptor underlying *stream* is associated with a pipe, FIFO or socket.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
