# wcstok()
Split a wide-character string with a delimiter string

## Synopsis
```c
#include <wchar.h>

wchar_t *wcstok(wchar_t *wcs, const wchar_t *delim, wchar_t **ptr);
```

## Arguments

- **wcs**
A pointer to the string.
&nbsp;
- **delim**
A pointer to the delimiter string.
&nbsp;
- **ptr**
It is used internally by wcstok() to save and restore the context between successive calls.

## Library
libc

## Description
wcstok() splits the string *str* into a list of tokens with the delimiter *delim*. When first calling this function, the first parameter should be the pointer to the string. In the following calls, the first parameter must be NULL. For each call, the delimiter *delim* may be different. A null-terminated string is returned as token every time wcstok() called and *delim* is not in the token. wcstok() returns NULL if no more tokens can be found.

## Returns

- **not NULL**
Return a pointer to the token found.
&nbsp;
- **NULL**
No more tokens can be found.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO