# strnlen()
Return the length of a string with a maximum length

## Synopsis
```c
#include <strings.h>

size_t strnlen(const char *s, size_t maxlen);
```

## Arguments

- **s**
A pointer to the string.
&nbsp;
- **maxlen**
The maximum number of characters to scan for the end of string marker before returning.

## Library
libc

## Description
Return the length of the string *s*, not including the terminating null byte ('\0'). The result is at most *maxlen*.

## Returns
The length of *s*, at most *maxlen*.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
