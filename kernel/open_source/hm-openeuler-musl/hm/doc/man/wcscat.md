# wcscat()
Concatenate two wide-character strings

## Synopsis
```c
#include <wchar.h>

wchar_t *wcscat(wchar_t *dest, const wchar_t *src);
```

## Arguments

- **dest**
A pointer to the destination buffer.
&nbsp;
- **src**
A pointer to the source buffer.

## Description
wcscat() appends the wide-character string in *src* to *dest* and overwrites the terminating null wide character (L'\0') of *dest*, finally it adds a new terminating null wide byte.
&nbsp;

When *src* and *dest* overlap, the result is not correct. The programmer must ensure the buffer size of *dest* is larger enough otherwise the function behavior is unpredictable and it may cause security problems.

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