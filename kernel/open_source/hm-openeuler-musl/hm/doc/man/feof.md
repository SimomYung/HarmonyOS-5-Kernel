# feof()
Test if the end-of-file indicator on a stream is set

## Synopsis
```c
#include <stdio.h>

int feof(FILE *stream);
```

## Arguments 
- **stream**
A stream pointer refers to a *FILE* structure.

## Library
libc

## Description
The feof() function tests if the end-of-file indicator for the stream pointed to by *stream* is set.

## Returns
- **0**
The end-of-file indicator is not set.
&nbsp;
- **non-zero**
The end-of-file indicator for *stream* is set.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

