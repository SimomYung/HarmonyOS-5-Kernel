# iconv\_close()
Deallocate a character code set converter

## Synopsis
```c
#include <iconv.h>

int iconv_close(iconv_t cd);
```

## Arguments
- **cd**
A conversion descriptor previously allocated by iconv\_open()

## Library
libc

## Description
The iconv\_close() function deallocates the conversion descriptor *cd* and any resources used by the converter.

## Returns
The iconv\_close() function always returns 0.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
