# memchr(), memrchr()
Find a character in the memory with conditions

## Synopsis
```c
#include <string.h>

void *memchr(const void *s, int c, size_t n);

void *memrchr(const void *s, int c, size_t n);
```

## Arguments

- **s**
A pointer to the memory.
&nbsp;
- **c**
The character to be found.
&nbsp;
- **n**
The number of bytes that will be scanned at most.

## Library
libc

## Description
memchr() scans at most *n* bytes of *s* and return a pointer to the first location when *c* is met.
&nbsp;

memrchr() scans at most *n* bytes of *s* and return a pointer to the last location when *c* is met.

## Returns

- **not NULL**
A pointer to the matched *c* in *s*.
&nbsp;
- **NULL**
*c* could not be found.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
