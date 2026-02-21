# newlocale()
Create a new locale or modify an existing locale

## Synopsis
```c
#include <locale.h>

locale_t newlocale(int mask, const char *locale, locale_t base);
```

## Arguments
- **mask**
The *mask* argument indicates the target locale categories, its value should be constructed by a bitwise-inclusive OR of the symbolic constants LC\_CTYPE\_MASK , LC\_NUMERIC\_MASK , LC\_TIME\_MASK , LC\_COLLATE\_MASK , LC\_MONETARY\_MASK , and LC\_MESSAGES\_MASK.
&nbsp;
- **locale**
For each category specified in *mask*, values in *locale* are used to create new locale or replace corresponding value of the existing locale.
&nbsp;
- **base**
A new locale is created with default value("POSIX" locale) when *base* is (locale\_t) 0, otherwise the new locale is based on *base*.

## Library
libc

## Description
The newlocale() function creates a new locale or modifies an existing locale based on the *base* argument. On success, the modification to *base* is undefined, thus the caller should use the result of newlocale() and make no access to *base*. As the *base* may be freed by newlocale(), it must not be the special locale object LC\_GLOBAL\_LOCALE, otherwise the behavior is undefined.

## Returns
If successful, the newlocale() shall return a handle which the caller may use on subsequent calls to duplocale(), freelocale(), and other functions taking a locale\_t argument.
If failure occurs, the newlocale() function shall return (locale\_t) 0 and set errno to indicate the error.

## Errors
- **ENOMEM**
No enough memory left to create the locale object or load the locale data.
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
