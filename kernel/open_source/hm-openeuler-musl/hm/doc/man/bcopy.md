# bcopy()
Copy some bytes from a byte sequence

## Synopsis
```c
#include <strings.h>

void bcopy(const void *src, void *dest, size_t n);
```

## Arguments

- **src**
A pointer to the source buffer.
&nbsp;
- **dest**
A pointer to the destination buffer.
&nbsp;
- **n**
The length of the sequences that is needed to copy.

## Library
libc

## Description
Copy *n* bytes from *src* to *dest*. When *src* and *dest* overlap, the result is also correct.

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