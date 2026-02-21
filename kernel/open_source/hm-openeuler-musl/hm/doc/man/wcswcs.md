# wcswcs()
Find the first wide character in a wide-character string which belongs to a set

## Synopsis
```c
#include <wchar.h>

wchar_t *wcswcs(const wchar_t *ws1, const wchar_t *ws2);
```

## Arguments

- **ws1**
A pointer to the string.
&nbsp;
- **ws2**
A pointer to the set of wide characters that are acceptable.

## Library
libc

## Description
Find the first wide character in the wide-character string *ws1* which belongs to the set *ws2*.

## Returns

- **not NULL**
A pointer to the first character in the string *ws1* which belongs to the set *ws2*. If *ws2* is an empty string, wcswcs() returns *ws1*.
&nbsp;
- **NULL**
None of the character in *ws1* belongs to the set *ws2*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
