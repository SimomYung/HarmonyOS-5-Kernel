# wcsspn()
Calculate the length of a substring that all of the wide characters in the substring belong to a set

## Synopsis
```c
#include <wchar.h>

size_t wcsspn(const wchar_t *wcs, const wchar_t *accept);
```

## Arguments

- **wcs**
A pointer to the string.
&nbsp;
- **accept**
A pointer to the set of wide characters that are acceptable.

## Library
libc

## Description
wcsspn() calculates the number of continuous wide characters in *wcs* that all of these wide characters are one of *accept*.

## Returns
wcsspn() returns the number of continuous wide characters in *wcs* that all of these wide characters are one of *accept*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO