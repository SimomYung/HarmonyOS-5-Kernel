# printf()
Print the formatted output

## Synopsis
```c
#include <stdio.h>

int printf(const char *restrict format, ...);
```

## Arguments 
- **format**
The string shall be printed associated with the *format*.

## Library
libc

## Description
The printf() function places the output on the stdout stream in specified format.

## Returns
Print the output successfully, printf() returns the number of bytes which are transmitted.

When an error occurs, a negative value is returned and *errno* is set.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

