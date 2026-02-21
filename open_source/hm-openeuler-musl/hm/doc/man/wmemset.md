# wmemset()
Fill the wide character array with a specified wide character

## Synopsis
```c
#include <wchar.h>

wchar_t *wmemset(wchar_t *wcs, wchar_t wc, size_t n);
```

## Arguments

- **wcs**
Target wide character array to be filled.
&nbsp;
- **wc**
The wide character that will be used to fill the target wide character array.
&nbsp;
- **n**
The wide character *wc* will be set from *wcs* to (*wcs* + *n* - 1).


## Library
libc

## Description
The wmemmet() function use *wc* to set the wide character array *wcs*, from *wcs* to (*wcs* + *n* - 1).

## Returns

- **wcs**
A pointer to target wide character array will be returned.


## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
