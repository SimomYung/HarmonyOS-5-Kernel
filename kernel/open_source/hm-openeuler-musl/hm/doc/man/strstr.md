# strstr(), strcasestr()
Find a substring

## Synopsis
```c
#include <strings.h>

char *strstr(const char *haystack, const char *needle);

#define _GNU_SOURCE
#include <strings.h>

char *strcasestr(const char *haystack, const char *needle);
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
strstr() scans the string in *haystack* and returns the first location when the substring *needle* is found. The terminating null bytes ('\0') are not compared.
&nbsp;

strcasestr() is like strstr(), except it ignores the case of the characters.

## Returns
- **not NULL**
A pointer to the matched *needle* in *haystack*.
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
