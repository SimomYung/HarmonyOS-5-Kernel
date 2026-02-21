# iconv\_open()
Allocate a character code set converter

## Synopsis
```c
#include <iconv.h>

iconv_t iconv_open(const char *tocode, const char *fromcode);
```

## Arguments
- **tocode**
Specifies which codeset the returned conversion descriptor should indicate as the target codeset.
&nbsp;
- **fromcode**
Specifies which codeset the returned conversion descriptor should indicate as the input codeset.

## Library
libc

## Description
The iconv\_open() allocate a code set converter. The code set converter is used by the iconv() to convert characters from one code set to another. The iconv\_open() finds the converter that performs the character code set conversion specified by the *tocode* and *fromcode* parameters, initializes that converter, and returns a conversion descriptor of type iconv\_t to identify the code set converter.

## Returns
On success, the iconv\_open() returns a conversion descriptor of type iconv\_t. When error occurs, the iconv\_open() returns (iconv\_t) -1 and set errno to indicate the error.

## Errors
- **EINVAL**
The *tocode* and *fromcode* are not supported.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
