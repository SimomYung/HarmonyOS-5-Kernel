# towlower()

Converts the given wide character to lowercase

## Synopsis

```c
#include <wctype.h>

wint_t towlower(wint_t wc);
```

## Arguments

- **wc**
  The wide character to be converted.

## Library

libc

## Description

The towlower() function converts the given wide character to lowercase.
&nbsp;
If the value of *wc* can not be represented as *wchar_t* and is not equal to WEOF, the behavior is undefined.

## Returns

The towlower() function will return the lowercase wide letter corresponding to *wc*. Otherwise, *wc* will not change.

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO