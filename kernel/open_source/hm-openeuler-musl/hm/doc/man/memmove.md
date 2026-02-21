# memmove()
Copy some bytes from a memory area

## Synopsis
```c
#include <string.h>

void *memmove(void *dest, const void *src, size_t n);
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
Copy *n* bytes from *src* to *dest*. When *src* and *dest* overlap, the result is also correct.

## Returns
A pointer to *dest*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
