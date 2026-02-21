# strdup(), strndup()
Duplicate a string

## Synopsis
```c
#include <strings.h>

char *strdup(const char *s);

char *strndup(const char *s, size_t n);
```

## Arguments

- **s**
A pointer to the original buffer.
&nbsp;
- **n**
The number of characters that are needed to copy at most.

## Library
libc

## Description

strdup() duplicates a copy of *s*.
&nbsp;

strndup() is like strdup(), but it copies at most *n* characters. Only *n* characters are copied when the length of *s* is larger than *n*, and a terminating null byte ('\0') is added at the end.

## Returns

- **not NULL**
A pointer to the duplicated string.
&nbsp;

- **NULL**
Memory available is not enough.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO