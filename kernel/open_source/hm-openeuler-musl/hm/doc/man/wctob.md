# wctob()
Convert a wide character into a single-byte character

## Synopsis
```c
#include <wchar.h>

int wctob(wint_t c);
```

## Arguments
- **c**
The wide character to be converted.

## Library
libc

## Description
The wctob() function returns the single-byte representation of a wide character.

## Returns
- **EOF**
*c* isn't a valid single-byte character.
&nbsp;
- **x**
The single-byte representation

## Errors
N/A

## Examples
&nbsp;

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
