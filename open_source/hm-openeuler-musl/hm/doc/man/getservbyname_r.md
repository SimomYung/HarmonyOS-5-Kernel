# getservbyname_r()

The reentrant version of getservbyname()

## Synopsis

```c
#include <netdb.h>

int getservbyname_r(const char *name, const char *proto,
                struct servent *result_buf, char *buf,
                size_t buflen, struct servent **result);
```

## Arguments

- **name**
  Pointer to a service name string.
  &nbsp;
- **proto**
  A pointer to a null-terminated protocol name.
  &nbsp;
- **result_buf**
  A pointer to servent structure, which will be filled in on success.
  &nbsp;
- **buf**
  The pointer to a user allocated temporary work buffer. It is used to store the string fields pointed to by the returned servent structure.
  &nbsp;
- **buflen**
  The temporary work buffer size.
  &nbsp;
- **result**
  If the function call successfully obtains a service record, then \**result* is set pointing to *result_buf*; otherwise, \**result* is set to NULL.


## Library

libc

## Description

getservbyname_r() is the reentrant version of getservbyname(). The caller supplies a *buf* of size *buflen* which will be filled with the returned structure servent in on success.

## Returns

On success, these functions return 0. On error, they return one of the positive error numbers listed in errors. If record not found, *result* will be set to NULL.
&nbsp;

The return value can be as follows:

- **ERANGE**
  *buf* is too small. Try again with a larger buffer (and increased *buflen*).

## Errors

N/A

## Example

&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
