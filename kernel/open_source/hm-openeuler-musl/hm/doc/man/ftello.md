# ftello(), ftello64()
Obtain a file offset in a stream

## Synopsis
```c
#include <stdio.h>

off_t ftello(FILE *stream);
off_t ftello64(FILE *stream);
```

## Arguments-
- **stream**
The stream is pointed to by *stream*.

## Library
libc

## Description
The ftello(), ftello64() function obtains the current value of the file-position indicator for the stream pointed to by *stream*.

## Returns
ftello(), ftello64() successfully, the current value of the file-position indicator for the stream from the beginning of the file successfully.

An error occurs, -1 is returned and the *errno* is set.

## Errors
- **EBADF**
The file descriptor underlying *stream* is not an open file descriptor.
&nbsp;
- **EOVERFLOW**
The current file offset can not be represented correctly in an object of type off_t.
&nbsp;
- **ESPIPE**
The file descriptor underlying stream is associated with a pipe, FIFO, or socket.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
