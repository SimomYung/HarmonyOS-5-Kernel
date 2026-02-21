# mbsrtowcs()
Convert a multibyte-character string into a wide-character string (restartable)

## Synopsis
```c
#include <wchar.h>

size_t mbsrtowcs(wchar_t * dst, const char ** src,
                 size_t n, mbstate_t * ps);
```

## Arguments
- **dst**
A pointer to a buffer where the function can store the wide-character string.
&nbsp;
- **src**
The string of multibyte characters to be converted.
&nbsp;
- **n**
The maximum number of wide characters to store in the buffer that *dst* points to.
&nbsp;
- **ps**
An internal pointer that lets mbsrtowcs() be a restartable version of mbstowcs(); if *ps* is NULL, mbsrtowcs() uses its own internal variable.

## Library
libc

## Description
The mbsrtowcs() function converts a string of multibyte characters pointed to by *src* into the corresponding wide characters pointed to by *dst*, to a maximum of *n* wide characters, including the terminating NULL character. The function converts each character as if by a call to mbtowc() and stops early if a sequence of bytes doesn't conform to a valid character or converting the next character would exceed the limit of *n* wide characters.

## Returns
- **(size_t)-1**
Failure; invalid wide-character code.
&nbsp;
- **x**
Success; the number of total characters successfully converted, not including the terminating NULL character.

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
