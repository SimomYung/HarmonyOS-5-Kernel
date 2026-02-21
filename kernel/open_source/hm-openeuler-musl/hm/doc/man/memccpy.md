# memccpy()
Copy some bytes from a byte sequence with conditions

## Synopsis
```c
#include <string.h>

void *memccpy(void *__restrict dest, const void *__restrict src, int c, size_t n);
```

## Arguments

- **src**
A pointer to the source buffer.
&nbsp;
- **dest**
A pointer to the destination buffer.
&nbsp;
- **c**
The character indicating the finish of copying.
&nbsp;
- **n**
The number of the bytes that is needed to copy at most.

## Library
libc

## Description
Copy at most *n* bytes from *src* to *dest*. When *c* is met, copying is stopped.
&nbsp;

When *src* and *dest* overlap, the result is undefined.

## Returns

- **not NULL**
A pointer to the next character of *c* in *dest*.
&nbsp;
- **NULL**
*c* could not be found in the first *n* characters of *src*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
