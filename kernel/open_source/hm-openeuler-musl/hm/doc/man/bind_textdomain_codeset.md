# bind_textdomain_codeset()
Set encoding of message translations

## Synopsis
```c
#include <libintl.h>

char *bind_textdomain_codeset(const char *domainname, const char *codeset);
```

## Arguments
- **domainname**
Specify the domain whose codeset you want to config for translations.
&nbsp;
- **codeset**
The codeset to be set.

## Library
- libc

## Description
The bind_textdomain_codeset() function sets the output codeset for message catalogs for domain *domainname*.
&nbsp;
Currently, only UTF-8 is supported and any other codeset is treated as invalid. The argument *domainname* is ignored.

## Returns
The function bind_textdomain_codeset() returns the current codeset for the domain *domainname*.
&nbsp;
Since only UTF-8 is supported currently, the default codeset, which is NULL, is always the current codeset and NULL is returned always.

## Errors
- **EINVAL**
If *codeset* is not UTF-8, the errno is set to *EINVAL*.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
