# wcslen()
Return the length of a wide-character string

## Synopsis
```c
#include <wchar.h>

size_t wcslen(const wchar_t *s);
```

## Arguments

- **s**
A pointer to the string.

## Library
libc

## Description
Return the length of the wide-character string *s*, not including the terminating null wide byte (L'\0').

## Returns
The number of wide characters in *s*.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO