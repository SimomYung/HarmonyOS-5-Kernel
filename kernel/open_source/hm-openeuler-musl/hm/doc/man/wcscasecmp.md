# wcscasecmp(), wcscasecmp_l(), wcsncasecmp(), wcsncasecmp_l()
Compare two wide-character strings regardless of case

## Synopsis
```c
#include <wchar.h>

int wcscasecmp(const wchar_t *s1, const wchar_t *s2);

int wcsncasecmp(const wchar_t *s1, const wchar_t *s2, size_t n);

int wcscasecmp_l(const wchar_t *s1, const wchar_t *s2, locale_t locale);

int wcsncasecmp_l(const wchar_t *s1, const wchar_t *s2, size_t n, locale_t locale);
```

## Arguments

- **s1**
A pointer to one of the string.
&nbsp;
- **s2**
A pointer to the other one of the string.
&nbsp;
- **n**
The number of characters that are needed to compare.
&nbsp;
- **locale**
The locale information.

## Library
libc

## Description
wcscasecmp() and wcscasecmp_l() checks *s1* and *s2* to see if they are equal or not regardless of the case of wide characters. If they are equal then the functions return 0. Otherwise a nonzero result is returned.
&nbsp;

wcsncasecmp() and wcsncasecmp_l() are like wcscasecmp() and wcscasecmp_l(), except they compare only the first *n* wide characters of *s1*.
&nbsp;

wcscasecmp_l() and wcsncasecmp_l() use *locale* to determine the case of wide characters. Instead, wcscasecmp() and wcsncasecmp() use the current locale. Now, *locale* only support C.UTF-8.

## Returns
- **0**
The contents of two strings are equal.
&nbsp;

- **non-zero**
The contents of two strings are not equal. If the length of *s1* is larger than *s2* then the functions return a positive integer, otherwise return a negative integer.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
