# wcspbrk()
Find the first wide character in a wide-character string which belongs to a set

## Synopsis
```c
#include <strings.h>

wchar_t *wcspbrk(const wchar_t *wcs, const wchar_t *accept);
```

## Arguments

- **wcs**
A pointer to the string.
&nbsp;
- **accept**
A pointer to the set of wide characters that are acceptable.

## Description
Find the first wide character in the string *s* which belongs to the set *accept*.

## Returns

- **not NULL**
A pointer to the first wide character in the string *s* which belongs to the set *accept*.
&nbsp;
- **NULL**
None of the wide character in *s* belongs to the set *accept*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO