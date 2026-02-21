# strtok(), strtok_r()
Split a string with a delimiter string

## Synopsis
```c
#include <strings.h>

char *strtok(char *str, const char *delim);
char *strtok_r(char *str, const char *delim, char **saveptr);
```

## Arguments

- **str**
A pointer to the string.
&nbsp;
- **delim**
A pointer to the delimiter string.
&nbsp;
- **saveptr**
It is used internally by strtok_r() to save and restore the context between successive calls.

## Library
libc

## Description
strtok() splits the string *str* into a list of tokens with the delimiter *delim*. When first calling this function, the first parameter should be the pointer to the string. In the following calls, the first parameter must be NULL. For each call, the delimiter *delim* may be different. A null-terminated string is returned as token every time strtok() called and *delim* is not in the token. strtok() returns NULL if no more tokens can be found.

strtok_r() is like strtok(), except it is thread safe. When different *saveptr* are used, different strings can be split concurrently.

## Returns

- **not NULL**
Return a pointer to the token founded.
&nbsp;
- **NULL**
No more tokens can be founded.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
