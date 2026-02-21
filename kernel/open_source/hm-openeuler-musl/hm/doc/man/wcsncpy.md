# wcsncpy()
Copy a wide-character string with a given length

## Synopsis
```c
#include <wchar.h>

wchar_t *wcsncpy(wchar_t *dest, const wchar_t *src, size_t n);
```

## Arguments

- **dest**
A pointer to the destination buffer.
&nbsp;
- **src**
A pointer to the source buffer.
&nbsp;
- **n**
The length of the string that is needed to copy.

## Library
libc

## Description
wcsncpy() copies at most *n* wide characters including the terminating null wide character (L'\0') from *src* to *dest*. If the length of *src* is smaller than *n*, the remaining spaces in *dest* are filled with null wide bytes (L'\0') . If the length of *src* is equal to or larger than *n*, the string in *dest* is not ended by a terminating null wide byte (L'\0').
&nbsp;

When *src* and *dest* overlap, the result is not correct. The programmer must ensure the buffer size of *dest* is larger enough.

## Returns
Return a pointer to *dest*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
