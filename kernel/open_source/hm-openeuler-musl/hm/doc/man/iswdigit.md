# iswdigit()

Check to see if the wide character is a decimal digit character

## Synopsis

```c
#include <wctype.h>

int iswdigit(wint_t wc);
```

## Arguments

- **wc**
  The wide character to check.

## Library

libc

## Description

The iswdigit() function checks to see if the wide character is a decimal digit character.

The *wc* argument is a *wint_t*, the value of which the application shall ensure is a wide character code corresponding to a valid character in the locale used by the function, or equal to the value of the macro WEOF. If the argument has any other value, the behavior is undefined.

## Returns

- **non-zero**
  This wide character is a decimal digit character.
&nbsp;
- **0**
  This wide character is not a decimal digit character.

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
