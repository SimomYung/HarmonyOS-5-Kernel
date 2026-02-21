# wmemchr()
Find specified wide character in a wide character array

## Synopsis
```c
#include <wchar.h>

wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n);
```

## Arguments

- **s**
Point to target wide character array to be searched.
&nbsp;
- **c**
Target wide character.
&nbsp;
- **n**
Search target wide character from *s* to (*s* + *n* -1).


## Library
libc

## Description
The wmemchr() function searches wide character *c* in the wide character array *s* starting from *s* to (*s* + *n* -1).

## Returns

- **pointer to first found target wide character**
If found, returns a pointer to the first wide character found in the wide character array.
&nbsp;

- **NULL**
No target wide character was found in the wide character array.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
