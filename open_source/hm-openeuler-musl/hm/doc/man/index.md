# index(), rindex()
Find a character in the string

## Synopsis
```c
#include <strings.h>

char *index(const char *s, int c);

char *rindex(const char *s, int c);
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
index() returns a pointer to the first location in *s* when *c* is met.
&nbsp;

rindex() returns a pointer to the last location in *s* when *c* is met.
&nbsp;

The terminating null byte ('\0') is considered as part of *s*.

## Returns

- **not NULL**
A pointer to the matched *c* in *s*.
&nbsp;
- **NULL**
*c* could not be found.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
