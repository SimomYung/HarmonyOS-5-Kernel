# crypt_r()
Encrypt a password

## Synopsis
```c
#include <crypt.h>

char *crypt_r(const char *key, const char *salt, struct crypt_data *data);
```
## Arguments
- **key**
A NULL-terminated string (normally a password typed by a user).
&nbsp;
- **salt**
A two-character string chosen from the set [a-zA-Z0-9./]. This function doesn't validate the values for salt, and values outside this range may cause undefined behavior. This string is used to perturb the algorithm in one of 4096 different ways.
&nbsp;
- **data**
The structure pointed to by data is used to store result data and bookkeeping information.

## Library
libc

## Description
The crypt_r() is a reentrant version of crypt().

## Returns
On success, a pointer to the encrypted password is returned. On error, NULL is returned.

## Errors
- **EINVAL**
salt has the wrong format.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
