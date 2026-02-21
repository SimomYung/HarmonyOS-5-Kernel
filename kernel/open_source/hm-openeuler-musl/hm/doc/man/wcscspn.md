# wcscspn()
Calculate the length of a substring that all of the wide characters in the substring don't belong to a set

## Synopsis
```c
#include <wchar.h>

size_t wcscspn(const wchar_t *wcs, const wchar_t *reject);
```

## Arguments

- **wcs**
A pointer to the string.
&nbsp;
- **reject**
A pointer to the set of wide characters that are not acceptable.

## Library
libc

## Description
wcscspn() calculates the number of continuous wide characters in *wcs* that none of these wide characters is one of *reject*.
&nbsp;

## Returns
wcscspn() returns the number of continuous wide characters in *wcs* that none of these wide characters is one of *reject*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO