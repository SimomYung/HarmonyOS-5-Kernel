# setvbuf()
Set the buffer and buffering type for an opened file stream

## Synopsis
```c
#include <stdio.h>

int setvbuf(FILE *restrict f, char *restrict buf, int type, size_t size);
```

## Arguments

- **f**
An opened file stream to set the buffer for caching.
&nbsp;
- **buf**
A block of memory whose size is *size*.
&nbsp;
- **type**
The buffering type, _IONBF(unbuffered), _IOLBF(line buffered), and _IOFBF(block buffered).
&nbsp;
- **size**
The length of *buf*.

## Library
libc

## Description
There are three types of buffering _IONBF (unbuffered), _IOFBF(block buffered), and _IOLBF(line buffered). When an output stream is unbuffered, information appears on the destination file or terminal as soon as written; when it is block buffered many characters are saved up and written as a block; when it is line buffered characters are saved up until a newline is output or input is read from any stream attached to a terminal device. Except for unbuffered stream, the argument *buf* should be a buffer at least *size* bytes long; this buffer will be used to replace the current buffer.

## Returns
On success, setvbuf() returns 0. On error, -1 is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
