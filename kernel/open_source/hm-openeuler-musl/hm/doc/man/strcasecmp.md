# strcasecmp(), strncasecmp(), strcasecmp_l(), strncasecmp_l()
Compare two strings regardless of case

## Synopsis
```c
#include <strings.h>

int strcasecmp(const char *s1, const char *s2);

int strncasecmp(const char *s1, const char *s2, size_t n);

int strcasecmp_l(const char *s1, const char *s2, locale_t locale);

int strncasecmp_l(const char *s1, const char *s2, size_t n, locale_t locale);
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
strcasecmp() and strcasecmp_l() checks *s1* and *s2* to see if they are equal or not regardless of the case of characters. If they are equal then the functions return 0. Otherwise a nonzero result is returned.
&nbsp;

strncasecmp() and strncasecmp_l() are like strcasecmp() and strcasecmp_l(), except they compare only the first *n* characters of *s1*.
&nbsp;

strcasecmp_l() and strncasecmp_l() use *locale* to determine the case of characters. Instead, strcasecmp() and strncasecmp() use the current locale. Now, *locale* only support C.UTF-8.

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