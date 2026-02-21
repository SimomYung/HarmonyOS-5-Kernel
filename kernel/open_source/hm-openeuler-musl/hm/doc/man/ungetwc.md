# ungetwc()
Push a wide character back to a file stream

## Synopsis
```c
#include <wchar.h>

wint_t ungetwc(wint_t c, FILE *f);
```

## Arguments

- **c**
A wide character to be pushed to stream *f*.
&nbsp;
- **f**
A stream to push back to.

## Library
libc

## Description

ungetwc() pushes back a wide character to stream *f*, where it is available for subsequent read operations. Pushed-back characters will be returned in reverse order; only one pushback is guaranteed.

## Returns
On success, ungetwc() returns c. On error, WEOF is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
