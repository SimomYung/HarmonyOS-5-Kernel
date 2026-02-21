# wcsftime()
Convert a broken-down time into a wide-character string

## Synopsis
```c
#include <wchar.h>

size_t wcsftime(wchar_t *restrict wcs, size_t maxsize,
				const wchar_t *restrict format, const struct tm *restrict timeptr);
```

## Arguments
- **wcs**
Points to the initial element of an array of wide characters into which the generated output is to be placed.
&nbsp;
- **maxsize**
The maximum number of wide characters to be placed in the output array.
&nbsp;
- **format**
Points to the area where the wide-character conversion specifications string stores.
&nbsp;
- **timeptr**
Points to the structure where the broken-down time stores.

## Library
libc

## Description
The wcsftime() function does the same conversion as the strftime() function.

## Returns
The wcsftime() function returns the number of bytes (excluding the terminating null byte) placed in the array *wcs* when the  result  string does not exceed *maxsize* bytes. If the length of the result string exceeds *maxsize* bytes, then wcsftime() returns 0, and the contents of the array are undefined.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO