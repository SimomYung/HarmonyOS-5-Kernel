# wcsstr()
Find a substring

## Synopsis
```c
#include <wchar.h>

wchar_t *wcsstr(const wchar_t *haystack, const wchar_t *needle);
```

## Arguments

- **haystack**
A pointer to the string to find *needle*.
&nbsp;
- **needle**
A pointer to the string that is needed to find.

## Library
libc

## Description
wcsstr() scans the string in *haystack* and returns the first location when the substring *needle* is found. The terminating null wide bytes (L'\0') are not compared.

## Returns
- **not NULL**
A pointer to the matched *needle* in *haystack*. If *needle* is an empty string, wcsstr() returns *haystack* itself.
&nbsp;
- **NULL**
*needle* could not be found.

## Errors
N/A

## Example
&nbsp;

## Classification

- **strstr**
1003.1-2017
&nbsp;
- **strcasestr**
UNIX

## Function Safety
TODO
