# bcmp()
Check whether two byte sequences are equal or not

## Synopsis
```c
#include <strings.h>

int bcmp(const void *s1, const void *s2, size_t n);
```

## Arguments

- **s1**
A pointer to one of the byte sequences.
&nbsp;
- **s2**
A pointer to the other one of the byte sequences.
&nbsp;
- **n**
The length of the sequences that is needed to check.

## Library
libc

## Description
Check the first *n* bytes of *s1* and *s2* to see if they are equal or not. If they are equal or *n* is 0 then bcmp() returns 0. Otherwise a nonzero result is returned.

## Returns

- **0**
Two byte sequences are equal or *n* is 0.
&nbsp;

- **non-zero**
Two byte sequences are not equal.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO