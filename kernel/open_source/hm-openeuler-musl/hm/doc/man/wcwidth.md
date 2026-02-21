# wcwidth()

Determine the number of column positions of a wide character code

## Synopsis

```c
#include <wchar.h>

wint_t wcwidth(wint_t wc);
```

## Arguments

- **wc**
  The wide character to be detected.

## Library

libc

## Description

The function determines the number of column positions of *wc*.
&nbsp;
If the value of *wc* can not be represented as *wchar_t*, the behavior is undefined.

## Returns

The function will return the number of column positions of *wc*.
&nbsp;
If *wc* is a printable wide character, the function will return the number of column positions to be occupied by the wide character code *wc*.
&nbsp;
if *wc* is a null wide character, 0 is returned. Otherwise, -1 is returned.

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO