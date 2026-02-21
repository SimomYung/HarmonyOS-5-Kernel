# bindtextdomain()
Set the base directory of the domain to be used in dgettext()

## Synopsis
```c
#include <libintl.h>

char *bindtextdomain(const char *domainname, const char *dirname);

```

## Arguments
- **domainname**
Specify the domain to be set.
&nbsp;
- **dirname**
The new directory to be set for the domain.

## Library
- libc

## Description
The domain will be expected at the pathnames *dirname/locale/category/domainname.mo*, where *locale* is a locale name and *category* is a locale facet such as *LC_MESSAGES* and *domainname.mo* is a set of messages.

## Returns

- **dirname**
The dirname set for the domain is returned on success.
&nbsp;
- **original dirname**
The original dirname for the domain is returned if *dirname* is NULL.
&nbsp;
- **NULL**
Invalid input or not enough memory cause the set failed and NULL is returned.

## Errors

- **ENOMEM**
Not enough memory available.

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
