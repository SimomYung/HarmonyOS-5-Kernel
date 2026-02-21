# strpbrk()
Find the first character in a string which belongs to a set

## Synopsis
```c
#include <strings.h>

char *strpbrk(const char *s, const char *accept);
```

## Arguments

- **s**
A pointer to the string.
&nbsp;
- **accept**
A pointer to the set of characters that are acceptable.

## Library
libc

## Description
Find the first character in the string *s* which belongs to the set *accept*.

## Returns

- **not NULL**
A pointer to the first character in the string *s* which belongs to the set *accept*.
&nbsp;
- **NULL**
None of the character in *s* belongs to the set *accept*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO