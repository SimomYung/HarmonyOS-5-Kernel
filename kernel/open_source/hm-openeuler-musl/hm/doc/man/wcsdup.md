# wcsdup()
Duplicate a wide-character string

## Synopsis
```c
#include <wchar.h>

wchar_t *wcsdup(const wchar_t *s);
```

## Arguments

- **s**
A pointer to the original string.

## Library
libc

## Description
wcsdup() duplicates a copy of *s*.

## Returns

- **a pointer to the new wide-character string**
Success.
&nbsp;

- **NULL**
An error occurred.

## Errors
- **ENOMEM**
The available memory is insufficient.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
