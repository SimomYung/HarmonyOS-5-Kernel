# nl\_langinfo()
Return language information defined in the current locale

## Synopsis
```c
#include <langinfo.h>

char *nl_langinfo(nl_item item);
```

## Arguments
- **item**
The *item* argument specifies the expected value that the caller wants to query in current locale.

## Library
libc

## Description
The nl\_langinfo() returns a value which is specified by the *item* from current locale.

## Returns
In a locale where language information data is not defined, the nl\_langinfo() returns a pointer to the corresponding string in the C locale. In all locales, the nl\_langinfo() returns a pointer to an empty string if the *item* parameter contains an invalid setting.
&nbsp;
The nl\_langinfo() returns a pointer to a static area, thus it might be overwritten by a subsequent call to nl\_langinfo().

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
