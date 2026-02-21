# nan(), nanf(), nanl()
Return quiet NaN

## Synopsis

```c
#include <math.h>

double nan(const char *tagp);
float nanf(const char *tagp);
long double nanl(const char *tagp);
```

## Arguments

- **tagp**
  pointer to a pointer to character.


## Library
libc

## Description
The function call nan("n-char-sequence") is equivalent to:
```
strtod("NAN(n-char-sequence)", (char **) NULL);
```
Similarly, calls to nanf() and nanl() are equivalent to similar calls to strtof and strtold.

## Returns

The quiet NaN value that corresponds to the identifying string *tagp* or zero if the implementation does not support quiet NaN.


## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO