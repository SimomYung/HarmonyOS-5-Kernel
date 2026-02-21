# setbuf()
Set the buffer for an opened file stream

## Synopsis
```c
#include <stdio.h>

void setbuf(FILE *restrict f, char *restrict buf);
```

## Arguments

- **f**
An opened file stream to set the buffer for caching.
&nbsp;
- **buf**
A block of memory whose size is not less than the macro BUFSIZ.

## Library
libc

## Description
setbuf() is called before any input and output on the opened stream. If the argument *buf* is null, information appears on the destination file or terminal as soon as written.

## Returns
N/A

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
