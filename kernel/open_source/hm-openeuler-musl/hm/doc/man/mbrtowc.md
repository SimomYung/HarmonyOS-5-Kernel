# mbrtowc()
Convert a multibyte character into a wide character

## Synopsis
```c
#include <wchar.h>

size_t mbrtowc(wchar_t * pwc,
               const char * s,
               size_t n,
               mbstate_t * ps);
```

## Arguments
- **pwc**
A pointer to a wchar\_t object where the function can store the wide character.
&nbsp;
- **s**
A pointer to the multibyte character to be converted.
&nbsp;
- **n**
The maximum number of bytes in the multibyte character to be converted.
&nbsp;
- **ps**
An internal pointer that lets mbrtowc() be a restartable version of mbtowc(); if *ps* is NULL, mbrtowc() uses its own internal variable.

## Library
libc

## Description
The mbrtowc() function converts single multibyte characters pointed to by *s* into wide characters pointed to by *pwc*, to a maximum of *n* bytes (not characters). This function is affected by LC\_TYPE.

## Returns
- **(size_t)-2**
After converting all *n* characters, the resulting conversion state indicates an incomplete multibyte character.
&nbsp;
- **(size_t)-1**
The function detected an encoding error before completing the next multibyte character; the function sets errno to EILSEQ and leaves the resulting conversion state undefined.
&nbsp;
- **0**
The next completed character is a NULL character; the resulting conversion state is the same as the initial one.
&nbsp;
- **x**
The number of bytes needed to complete the next multibyte character, in which case the resulting conversion state indicates that *x* bytes have been converted.

## Errors
- **EILSEQ**
Invalid character sequence.
&nbsp;
- **EINVAL**
The *ps* argument points to an invalid object.

## Examples
&nbsp;

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
