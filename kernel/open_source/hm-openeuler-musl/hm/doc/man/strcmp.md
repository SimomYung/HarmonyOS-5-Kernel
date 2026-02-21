# strcmp(), strncmp()
Check whether two strings are equal or not

## Synopsis
```c
#include <strings.h>

int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, size_t n);
```

## Arguments

- **s1**
A pointer to one of the string.
&nbsp;
- **s2**
A pointer to the other one of the string.
&nbsp;
- **n**
The length of the string that is needed to check.

## Library
libc

## Description

strcmp() checks *s1* and *s2* to see if they are equal or not. If they are equal then strcmp() returns 0. Otherwise a nonzero result is returned.
&nbsp;

strncmp() is like strcmp(), except it checks the first *n* bytes of *s1* and *s2*.

## Returns

- **0**
Two strings are equal.
&nbsp;

- **non-zero**
Two strings are not equal. The sign is calculated by the difference of the first pair of characters that are different in *s1* and *s2*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO