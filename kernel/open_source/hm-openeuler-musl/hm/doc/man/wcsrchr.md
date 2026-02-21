# wcsrchr()
Find a wide character in a wide-character string

## Synopsis
```c
#include <wchar.h>

wchar_t *wcsrchr(const wchar_t *wcs, wchar_t wc);
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
wcsrchr() returns a pointer to the last location in *wcs* when *wc* is met.

## Returns
- **not NULL**
Return a pointer to the last location in *wcs* when *wc* is met.
&nbsp;

- **NULL**
When *wc* can not be found.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO