# fprintf()
Print a formatted output

## Synopsis
```c
#include <stdio.h>

int fprintf(FILE *restrict stream, const char *restrict format, ...);
```

## Arguments 
- **stream**
A named output stream.
&nbsp;
- **format**
The argument *format* converts the output to the indicated format.

## Library
libc

## Description
The fprintf() function places the output on the named output stream pointed to by *stream* in specified format.

## Returns
Print the output successfully, the number of bytes which are transmitted is returned.

When an error occurs, a negative value is returned and *errno* is set.

## Errors
- **EOVERFLOW**
The value to be returned is greater than {INT_MAX}.
&nbsp;
- **EINVAL**
The arguments are insufficient.
&nbsp;
- **ENOMEM**
The insufficient memory space is available.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
