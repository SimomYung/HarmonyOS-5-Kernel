# fgetwc()
Get a wide-character from a stream

## Synopsis
```c
#include <stdio.h>
#include <wchar.h>

wint_t fgetwc(FILE *stream);
```

## Arguments 
- **stream**
A stream pointer refers to a *FILE* structure.

## Library
libc

## Description
The fgetwc() function obtains the next character from a input stream pointed by *stream* and converts it to the corresponding wide-character code, then advances the associated file position indicator for the stream.

## Returns
Successful completion, the wide-character code of the character read from the input stream pointed to by *stream* is returned and converted to a type wint_t.

If the end-of-file indicator for the stream is set, or if the stream is at end-of-file, the end-of-file indicator for the stream shall be set and *WEOF* is returned. If a read error occurs, the error indicator for the stream shall be set, *WEOF* is returned and *errno* is set.  If an encoding error occurs, the error indicator for the stream shall be set, *WEOF* is returned and *errno* is set.

## Errors
- **EAGAIN**
The *O_NONBLOCK * is set for the file descriptor underlying *stream* and the caller will be delayed.
&nbsp;
- **EINTR**
The file descriptor underlying *stream* is not a valid file descriptor open for reading.
&nbsp;
- **EOVERFLOW**
Attempt to read at or beyond the offset maximum associated with the corresponding stream.
&nbsp;
- **ENOMEM**
Insufficient memory space is available.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

