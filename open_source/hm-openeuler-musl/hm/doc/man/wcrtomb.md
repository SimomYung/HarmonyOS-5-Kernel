# wcrtomb()
Convert a wide-character code to a character

## Synopsis
```c
#include <wchar.h>

size_t wcrtomb(char *__restrict s, wchar_t wc, mbstate_t *__restrict ps);
```

## Arguments
- **s**
NULL, or a pointer to a location where the function can store the multibyte character.
&nbsp;
- **wc**
The wide character to be converted.
&nbsp;
- **ps**
An internal pointer that lets wcrtomb() be a restartable version of wctomb(); if *ps* is NULL, wcrtomb() uses its own internal variable.

## Library
libc

## Description
The wcrtomb() function determines the number of bytes needed to represent the wide character *wc* as a multibyte character and stores the multibyte character in the location pointed to by *s*, to a maximum of MB\_CUR\_MAX bytes. This function is affected by LC\_CTYPE.

## Returns
The number of bytes stored, or (size\_t)-1 if the variable *wc* is an invalid wide-character code.

## Errors
- **EILSEQ**
Invalid wide-character code.
&nbsp;
- **EINVAL**
The variable ps points to an invalid conversion state.

## Examples
&nbsp;

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
