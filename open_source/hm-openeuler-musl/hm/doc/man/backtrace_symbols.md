# backtrace\_symbols()

Get the string that describes the function

## Synopsis

```c
#include <execinfo.h>

char **backtrace_symbols(void *const *buffer, int size);
```

## Arguments

- **buffer**
  The array that stores the addresses acquired by function backtrace().
  &nbsp;
- **size**
  The number of addresses stored in *buffer*.

## Library

libc

## Description

The backtrace\_symbols() function can get the strings which describe the addresses symbolically acquired by function backtrace().

## Returns

- **NULL**
  An error is occurred.
&nbsp;
- **address of an array**
  On success, backtrace\_symbols obtains the address of an array which stores a number of string pointers.

## Errors

N/A

## Example
&nbsp;

## Classification

Unix

## Function Safety

TODO
