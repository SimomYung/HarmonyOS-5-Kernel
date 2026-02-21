# mempcpy()
Copy some bytes from a memory area

## Synopsis
```c
#define _GNU_SOURCE
#include <strings.h>

void *mempcpy(void *dest, const void *src, size_t n);
```

## Arguments

- **dest**
A pointer to the destination buffer.
&nbsp;
- **src**
A pointer to the source buffer.
&nbsp;
- **n**
The number of bytes that are needed to copy.

## Library
libc

## Description
Copy *n* bytes from *src* to *dest*. It returns a pointer to the location next to the last written byte. So it is useful to copy in consecutive memory positions.

## Returns
*dest* + *n*

## Errors
N/A

## Example
&nbsp;

## Classification
UNIX

## Function Safety
TODO