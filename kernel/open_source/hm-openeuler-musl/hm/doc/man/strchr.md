# strchr(), strrchr(), strchrnul()
Find a character in a string

## Synopsis
```c
#include <strings.h>

char *strchr(const char *s, int c);

char *strrchr(const char *s, int c);

#define _GNU_SOURCE
#include <string.h>

char *strchrnul(const char *s, int c);
```

## Arguments

- **s**
A pointer to the string.
&nbsp;
- **c**
The character to be found.

## Library
libc

## Description
strchr() returns a pointer to the first location in *s* when *c* is met.
&nbsp;

strrchr() returns a pointer to the last location in *s* when *c* is met.
&nbsp;

strchrnul() is like strchr(), except when *c* can not be found, it return a pointer to the terminating null byte.
&nbsp;

The terminating null byte ('\0') is considered as part of *s*.

## Returns

strchr() and strrchr() return a pointer to the matched *c* in *s*, or NULL when c can not be found.

strchrnul() returns a pointer to the matched *c* in *s*, or it return a pointer to the terminating null byte when *c* can not be found.

## Errors
N/A

## Example
&nbsp;

## Classification

- **strchr**
1003.1-2017
&nbsp;
- **strrchr**
1003.1-2017
&nbsp;
- **strchrnul**
UNIX

## Function Safety
TODO
