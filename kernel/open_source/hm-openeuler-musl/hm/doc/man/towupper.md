# towupper()

Converts the given wide character to uppercase

## Synopsis

```c
#include <wctype.h>

wint_t towupper(wint_t wc);
```

## Arguments

- **wc**
  The wide character to be converted.

## Library

libc

## Description

The towupper() function converts the given wide character to uppercase.
&nbsp;
If the value of *wc* can not be represented as *wchar_t* and is not equal to WEOF, the behavior is undefined.

## Returns

The towupper() function will return the uppercase wide letter corresponding to *wc*. Otherwise, *wc* will not change.

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO