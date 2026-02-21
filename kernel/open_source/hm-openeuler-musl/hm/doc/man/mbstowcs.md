# mbstowcs()
Convert a character string to a wide-character string

## Synopsis
```c
#include <stdlib.h>

size_t mbstowcs(wchar_t *dest, const char *src, size_t n);
```
## Arguments

- **dest**
Pointer to wide character array where the wide string will be stored.
&nbsp;
- **src**
Pointer to the first element of a null-terminated multibyte string to convert.
&nbsp;
- **n**
Number of wide characters available in the array pointed to by *dest*.

## Library
libc

## Description
If *dest* is not NULL, the function converts the multi-byte string *src* into a wide character string starting with *dest*. Writes up to *n* wide characters to *dest*. The sequence of characters in the string *src* should start from the initial shift state.
&nbsp;
If an invalid multibyte sequence is encountered. In this case, (size_t) -1 will be returned.
&nbsp;
If *n* non-L'\0' wide characters have been stored in *dest*. In this case, the number of wide characters written to *dest* is returned.
&nbsp;
If the multi-byte string has been fully converted, including the terminating null character. In this case, the number of wide characters written to dest is returned, excluding the terminating null wide characters.
&nbsp;
The programmer must ensure that there is room for at least *n* wide characters at *dest*.

## Returns

The function returns the number of wide characters that make up the converted part of a wide-character string, excluding the terminating null-width characters. If an invalid multibyte sequence is encountered, (size_t) -1 is returned.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
