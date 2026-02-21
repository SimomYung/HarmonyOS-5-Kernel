# sethostname()
Set the hostname

## Synopsis
```c
#include <unistd.h>

int sethostname(const char *name, size_t len);
```

## Arguments

- **name**
Pointers to the area where the name string stores.
&nbsp;
- **len**
The number of bytes in *name*.

## Library
libc

## Description
The sethostname() function sets the hostname to the value given in the character array *name*.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
*name* is an invalid address.
&nbsp;
- **EINVAL**
*len* is negative or is larger than the maximum allowed size.
&nbsp;
- **EPERM**
The caller did not have the authority.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
