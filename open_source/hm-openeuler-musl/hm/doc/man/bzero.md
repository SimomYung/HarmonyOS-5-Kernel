# bzero()
Write some zero-valued bytes

## Synopsis
```c
#include <strings.h>

void bzero(void *s, size_t n);
```

## Arguments

- **s**
A pointer to the buffer.
&nbsp;
- **n**
The number of bytes in the sequence that is needed to set to zero ('\0').

## Library
libc

## Description
Set the first *n* bytes of *s* to zero.

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