# wcschr()
Find a wide character in a wide-character string

## Synopsis
```c
#include <wchar.h>

wchar_t *wcschr(const wchar_t *wcs, wchar_t wc);
```

## Arguments

- **wcs**
A pointer to the wide-character string.
&nbsp;
- **wc**
The wide character to be found.

## Library
libc

## Description
wcschr() returns a pointer to the first location in *wcs* when *wc* is met.

## Returns
- **not NULL**
Return a pointer to the first location in *wcs* when *wc* is met.
&nbsp;

- **NULL**
*wc* can not be found.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO