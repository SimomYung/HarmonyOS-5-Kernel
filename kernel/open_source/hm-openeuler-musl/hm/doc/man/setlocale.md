# setlocale()
Set or query the program's current locale

## Synopsis
```c
#include <locale.h>

char *setlocale(int category, const char *locale);
```


## Arguments
- **category**
Determine which parts of the program's current locale should be modified.

     |  Category    |   Governs |
     |  --------    |   ------- |
     |  LC_ALL      |   All of the locale |
     |  LC_COLLATE  |   String collation |
     |  LC_CTYPE    |   Character classification |
     |  LC_MESSAGES |   Localizable natural-language messages |
     |  LC_MONETARY |   Formatting of monetary values |
     |  LC_NUMERIC  |   Formatting of nonmonetary numeric values  |
     |  LC_TIME     |   Formatting of date and time values |

- **locale**
The argument locale is a pointer to a character string containing the required setting of category. Currently setlocale supports zh_CN.UTF-8, which requires the environment variable *MUSL_LOCPATH* to be set to */usr/share/locale* to specify the directory of locale data. Also *C*, *POSIX* and *C.UTF\-8* are supported.

## Library
- libc

## Description
If *locale* is an empty string, "", each part of the locale that should be modified is set according to the environment variables. First (regardless of category), the environment variable *LC_ALL* is inspected, next the environment variable with the same name as the category (see the table above), and finally the environment variable *LANG*. The first existing environment variable is used. If its value is not a valid locale specification, the locale is unchanged, and setlocale() returns NULL.
&nbsp;
The locale to be set should be named in the form language[_territory][.codeset][@modifier], where language is an ISO 639 language code, territory is an ISO 3166 country code, and codeset is a character set or encoding  identifier like ISO-8859-1 or UTF-8. Now musl supports only one codeset and it is UTF-8. And the codeset given cannot be recognized except that the locale to be set is C.UTF-8. In other words, just ignore the codeset and it is UTF-8 always.

## Returns

- **locale**
A successful call to setlocale() returns an opaque string that corresponds to the locale set.
&nbsp;
- **current locale**
If *locale* is NULL, the current locale is only queried, not modified.
&nbsp;
- **NULL**
If the *locale* or *category* are invalid, setlocale() returns NULL.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX

## Function Safety

TODO
