# putchar()
Write a character to stdout

## Synopsis
```c
#include <stdio.h>

int putchar(int c);
```
## Arguments

- **c**
A character to be written to stdout.

## Library
libc

## Description
Function putchar() writes a character to stdout. putchar(c) is the same as putc(c, stdout).

## Returns
On success, putchar() returns the character written as an unsigned char cast to an int. On failure, EOF is returned.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
