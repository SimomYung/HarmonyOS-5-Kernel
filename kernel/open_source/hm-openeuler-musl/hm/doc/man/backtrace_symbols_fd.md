# backtrace\_symbols\_fd()

Get the string that describes the function and write to fd

## Synopsis

```c
#include <execinfo.h>

void backtrace_symbols_fd(void *const *buffer, int size, int fd);
```

## Arguments

- **buffer**
  The array that stores the addresses acquired by function backtrace().
  &nbsp;
- **size**
  The number of addresses stored in *buffer*.
  &nbsp;
- **fd**
  The file descriptor to write into.

## Library

libc

## Description

The backtrace\_symbols\_fd() function can get the strings which describe the addresses symbolically acquired by function backtrace() and write to fd.

## Returns

N/A

## Errors

N/A

## Example
&nbsp;

## Classification

Unix

## Function Safety

Nonreentrant; Multi-thread Unsafe
