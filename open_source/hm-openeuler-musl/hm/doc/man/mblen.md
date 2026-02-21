# mblen()
Get number of bytes in a multibyte character

## Synopsis
```c
#include <stdlib.h>

int mblen(const char *s, size_t n);
```
## Arguments

- **s**
Pointer to the multibyte character.
&nbsp;
- **n**
Limit on the number of bytes in *s* that can be examined.

## Library
libc

## Description
If *s* is not NULL, the function checks up to *n* bytes of a multibyte string beginning with *s* and extracts the next complete multibyte character. If the multibyte character is not a null character, the number of bytes consumed from *s* is returned. If the multibyte character is a null width character, 0 is returned.
&nbsp;
If the *n* bytes starting with *s* do not contain a complete multibyte character, -1 is returned.
&nbsp;
If a multibyte string beginning with *s* contains an invalid multibyte sequence before the next full character, -1 is returned.

## Returns

If a non-null wide character is recognized, the function returns the number of bytes parsed from a multibyte sequence beginning with *s*. If a null wide character is recognized, 0 is returned. If an invalid multibyte sequence is encountered or a full multibyte character cannot be parsed, -1 is returned.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
