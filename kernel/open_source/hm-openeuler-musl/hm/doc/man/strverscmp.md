# strverscmp()
Check whether two version strings are equal or not

## Synopsis
```c
#define _GNU_SOURCE
#include <strings.h>

int strverscmp(const char *s1, const char *s2);
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
strverscmp() checks *s1* and *s2* to see if they are equal or not. If they are equal then strcmp() returns 0. Otherwise a nonzero result is returned. The sign of the result is calculated as follows. First find the first location where *s1* and *s2* are different. Then  translate the longest consecutive digit characters into number respectively. Finally the sign is calculated by the difference of the two number. If either of the string can't be translated into number, the result is returned as if strcmp() called.

## Returns

- **0**
Two strings are equal.
&nbsp;

- **non-zero**
Two strings are not equal. A positive number is returned when *s1* is earlier than *s2*. Otherwise a negative number is returned.

## Errors
N/A

## Example
&nbsp;

## Classification
UNIX

## Function Safety
TODO
