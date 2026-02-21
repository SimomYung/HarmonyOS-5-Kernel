# putwc(), fputwc()
Write a wide character to stream

## Synopsis
```c
#include <stdio.h>
#include <wchar.h>

wint_t putwc(wchar_t c, FILE *f);
wint_t fputwc(wchar_t c, FILE *f);
```
## Arguments

- **c**
A wide character to be written to stream.
&nbsp;
- **f**
File stream.

## Library
libc

## Description
putwc() writes a wide character c to stream f. The putwc() function is identical to fputwc().


## Returns

On success, return c. On error, return WEOF.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
