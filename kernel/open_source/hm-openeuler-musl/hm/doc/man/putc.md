# putc(), fputc()
Write a character to file

## Synopsis
```c
#include <stdio.h>

int putc(int c, FILE *f);
int fputc(int c, FILE *f);
```
## Arguments

- **c**
A character to be written.
&nbsp;
- **f**
An open file obtained with fopen.

## Library
libc

## Description
Function putc() writes a character to file that is obtained with fopen. The function fputc is the same as putc.


## Returns

On success, fputc() or putc() returns the character written as an unsigned char cast to an int on success. On failure, EOF is returned.

## Errors

N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
