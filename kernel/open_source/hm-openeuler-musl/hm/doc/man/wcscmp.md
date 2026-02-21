# wcscmp()
Check whether two wide-character strings are equal or not

## Synopsis
```c
#include <wchar.h>

int wcscmp(const wchar_t *s1, const wchar_t *s2);
```

## Arguments

- **s1**
A pointer to one of the string.
&nbsp;
- **s2**
A pointer to the other one of the string.

## Library
libc

## Description

wcscmp() checks *s1* and *s2* to see if they are equal or not. If they are equal then wcscmp() returns 0. Otherwise a nonzero result is returned.

## Returns

- **0**
Two strings are equal.
&nbsp;

- **non-zero**
Two strings are not equal. The sign is calculated by the difference of the first pair of wide characters that are different in *s1* and *s2*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO