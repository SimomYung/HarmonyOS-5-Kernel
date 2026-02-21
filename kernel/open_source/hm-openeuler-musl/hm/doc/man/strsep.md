# strsep()
Split a string with a delimiter character

## Synopsis
```c
#include <strings.h>

char *strsep(char **stringp, const char *delim);
```

## Arguments

- **stringp**
A pointer to the address of the string.
&nbsp;
- **delim**
A pointer to the delimiter string.

## Library
libc

## Description
strsep() splits the string *\*strstringp* into a list of tokens with one of the delimiter character in *delim*. A null-terminated string is returned as token every time strsep() called and *delim* is not in the token, then *\*strstringp* is pointed to the next character of the delimiter character. The token is the entire *\*strstringp* if no delimiter character can be found, and *\*strstringp* is set to NULL.

## Returns

- **not NULL**
Return a pointer to the token founded.
&nbsp;
- **NULL**
*\*strstringp* is NULL.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO