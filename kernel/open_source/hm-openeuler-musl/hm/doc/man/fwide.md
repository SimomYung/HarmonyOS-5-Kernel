# fwide()
Set the stream orientation

## Synopsis
```c
#include <wchar.h>

int fwide(FILE *stream, int mode);
```

## Arguments
- **stream**
The *stream* whose orientation is to be set.
&nbsp;
- **mode**
The argument *mode* indicates the mode that the stream-oriented will be set.

## Library
libc

## Description
The fwide() function determines the orientation of the stream pointed to by *stream*. If *mode* is greater than zero, it first attempts to make the stream wide-oriented. If *mode* is less than zero, it first attempts to make the stream byte-oriented. Otherwise, *mode* is zero and the function does not alter the orientation of the stream. If the orientation of the stream has already been determined, fwide() shall not change it.

## Returns
- **A positive Number**
If the system has wide-orientation, a value greater than zero is returned.
&nbsp;
- **0**
If the stream has no orientation, 0 is returned.
&nbsp;
- **A Negative Number**
If the stream has byte-oriention, a value less than zero is returned.

## Errors
- **EBADF**
The argument *stream* points to a invalid stream.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
