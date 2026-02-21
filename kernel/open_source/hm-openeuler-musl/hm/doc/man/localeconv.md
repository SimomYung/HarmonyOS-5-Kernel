# localeconv()
Return a pointer which points to the current locale structure

## Synopsis
```c
#include <locale.h>

struct lconv *localeconv(void);
```

## Arguments
N/A

## Library
libc

## Description
The localeconv() function returns a pointer to the current locale structure, which holds attributes related to categories LC\_MONETARY and LC\_NUMERIC. The structure definition is shown as below:
```c
struct lconv {
        char *decimal_point;
        char *thousands_sep;
        char *grouping;
        char *int_curr_symbol;
        char *currency_symbol;
        char *mon_decimal_point;
        char *mon_thousands_sep;
        char *mon_grouping;
        char *positive_sign;
        char *negative_sign;
        char int_frac_digits;
        char frac_digits;
        char p_cs_precedes;
        char p_sep_by_space;
        char n_cs_precedes;
        char n_sep_by_space;
        char p_sign_posn;
        char n_sign_posn;
        char int_p_cs_precedes;
        char int_p_sep_by_space;
        char int_n_cs_precedes;
        char int_n_sep_by_space;
        char int_p_sign_posn;
        char int_n_sign_posn;
};
```

## Returns
A pointer which points to the current locale structure.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
