# ungetc()
Push a character back to stream

## Synopsis
```c
#include <stdio.h>

int ungetc(int c, FILE *f);
```

## Arguments

- **c**
A character to be pushed to stream *f*.
&nbsp;
- **f**
A stream to push back to.

## Library
libc

## Description

ungetc() pushes c back to stream *f*, cast to unsigned char, where it is available for subsequent read operations. Pushed-back characters will be returned in reverse order; only one pushback is guaranteed.

## Returns
On success, ungetc() returns c. On error, EOF is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
