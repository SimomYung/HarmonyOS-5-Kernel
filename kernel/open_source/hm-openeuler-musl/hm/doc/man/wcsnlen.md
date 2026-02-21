# wcsnlen()
Return the length of a wide character string with a maximum length

## Synopsis
```c
#include <wchar.h>

size_t wcsnlen(const wchar_t *s, size_t maxlen);
```

## Arguments

- **s**
A pointer to the string.
&nbsp;
- **maxlen**
The maximum length.

## Library
libc

## Description
Return the length of the wide character string *s*, not including the terminating null wide byte (L'\0'). The result is at most *maxlen*.

## Returns
The length of *s*, at most *maxlen*.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO