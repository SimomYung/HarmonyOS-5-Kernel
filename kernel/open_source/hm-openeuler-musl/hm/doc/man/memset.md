# memset()
Fill memory with a given byte value

## Synopsis
```c
#include <string.h>

void *memset(void *s, int c, size_t n);
```

## Arguments

- **s**
A pointer to the buffer.
&nbsp;
- **c**
The given byte value.
&nbsp;
- **n**
The number of bytes that are needed to fill.

## Library
libc

## Description
Fill the first *n* bytes of *s* with a given byte value *c*.

## Returns
A pointer to *s*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
