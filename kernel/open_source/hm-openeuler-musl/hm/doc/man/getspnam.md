# getspnam(), getspnam\_r()
Get information about a user with a given name

## Synopsis
```c
#include <sys/types.h>
#include <shadow.h>

struct spwd *getspnam(const char *name);

int getspnam_r(const char *name, struct spwd *sp, char *buf, size_t size, struct spwd **res)
```
## Arguments
- **name**
The name of user.
&nbsp;
- **sp**
A pointer to a spwd structure where function can store entry.
&nbsp;
- **buf**
A block of memory that function can use to allocate storage referenced by spwd structure.
&nbsp;
- **size**
The size of block that *buf* points to, in characters.
&nbsp;
- **res**
The address of a pointer to a spwd structure pointer. If getspnam\_r() finds entry, it stores a pointer to *sp*; otherwise function stores a NULL pointer there.

## Library
libc

## Description
The getspnam() and getspnam\_r() functions allow a process to gain more knowledge about a user name. The getspnam() function uses a static buffer that's overwritten by each call.

## Returns
A pointer to an object of type struct spwd containing an entry from group database with a matching name, or NULL if an error occurred or function couldn't find a matching entry.

## Errors
- **EACCES**
The caller does not have permission to access shadow password file.
&nbsp;
- **EINVAL**
The name is invalid.
&nbsp;
- **ERANGE**
Supplied buf is too small, or size is too small.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
