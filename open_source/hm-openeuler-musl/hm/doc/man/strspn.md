# strspn(), strcspn()
Calculate the length of a substring that all of the characters in the substring belong to a set or not

## Synopsis
```c
#include <strings.h>

size_t strspn(const char *s, const char *accept);

size_t strcspn(const char *s, const char *reject);
```

## Arguments

- **s**
A pointer to the string.
&nbsp;
- **accept**
A pointer to the set of characters that are acceptable.
&nbsp;
- **reject**
A pointer to the set of characters that are not acceptable.

## Library
libc

## Description
strspn() calculates the number of continuous characters in *s* that all of these characters are one of *accept*.
&nbsp;

strcspn() calculates the number of continuous characters in *s* that none of these characters is one of *reject*.
&nbsp;

## Returns

strspn() returns the number of continuous characters in *s* that all of these characters are one of *accept*.
&nbsp;

strcspn() returns the number of continuous characters in *s* that none of these characters is  one of *reject*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO