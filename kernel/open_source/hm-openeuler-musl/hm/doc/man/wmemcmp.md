# wmemcmp()
Compare two wide character arrays

## Synopsis
```c
#include <wchar.h>

int wmemcmp(const wchar_t *s1, wchar_t *s2, size_t n);
```

## Arguments

- **s1**
Wide character array A.
&nbsp;
- **s2**
Wide character array B.
&nbsp;
- **n**
Compare the two wide character arrays *s1* and *s2* for *n* wide characters.


## Library
libc

## Description
The wmemcmp() function compares two wide character arrays. The first array starts at *s1* and ends at (*s1* + *n* - 1), and the second array starts at *s2* and ends at (*s2* + *n* - 1).

## Returns

- **Zero**
If the two wide character arrays equal, returns 0.
&nbsp;

- **Integer greater than 0**
For the first different character found, assuming that the index is i (i < *n*), if *s1[i]* is greater than *s2[i]*, an integer greater than 0 is returned.
&nbsp;

- **Integer less than 0**
For the first different character found, assuming that the index is i (i < *n*), if *s1[i]* is less than *s2[i]*, an integer less than 0 is returned.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
