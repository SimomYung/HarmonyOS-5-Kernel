# wcsnrtombs()
Convert a wide-character string into a multibyte character string

## Synopsis
```c
#include <wchar.h>

size_t wcsnrtombs(char *dest, const wchar_t **src, size_t nwc,
                  size_t len, mbstate_t *ps);
```

## Arguments
- **dest**
A pointer to a buffer where the function can store the multibyte string.
&nbsp;
- **src**
A pointer to the wide-character string to be converted.
&nbsp;
- **nwc**
The maximum number of wide characters in src can be converted.
&nbsp;
- **len**
The maximum number of multibyte characters to be stored.
&nbsp;
- **ps**
An internal pointer that lets wcsrtombs() be a restartable version of wcstombs(); if *ps* is NULL, wcsrtombs() uses its own internal variable.

## Library
libc

## Description
The wcsnrtombs() function converts a string of wide-characters pointed to by *src* into the corresponding multi-byte characters pointed to by *dst*. No more than *len* bytes are stored, including the terminating NULL character, and the number of wide characters to be converted, starting at *\*src*, is limited to *nwc*.

## Returns
- **-1**
An invalid wide character is encountered.
&nbsp;
- **x**
The number of bytes that make up the converted part of multibyte sequence, not including the terminating NULL byte.

## Errors
- **EILSEQ**
An invalid wide character is encountered.

## Examples
&nbsp;

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
