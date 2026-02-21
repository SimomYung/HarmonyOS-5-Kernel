# iconv()
Convert a string of characters in one character code set to another

## Synopsis
```c
#include <iconv.h>

size_t iconv(iconv_t cd, char **restrict inbuf, size_t *restrict inbytesleft, char **restrict outbuf, size_t *restrict outbytesleft);
```

## Arguments

- **cd**
The conversion descriptor that points to the correct code set converter.
&nbsp;
- **inbuf**
A pointer of a buffer that contains the number of bytes in the *inbytesleft* parameter to be converted.
&nbsp;
- **inbytesleft**
A pointer of an integer that contains the number of bytes in the *inbuf* parameter.
&nbsp;
- **outbuf**
A pointer of a buffer that contains the number of bytes in the *outbytesleft* parameter that has been converted.
&nbsp;
- **outbytesleft**
A pointer of an integer that contains the number of bytes in the *outbuf* parameter.

## Library
libc

## Description
The iconv() converts the string specified by the *inbuf* parameter into a different code set and returns the results in the *outbuf* parameter. The required conversion method is identified by the *cd* argument, which must be valid conversion descriptor returned by a previous successful call to the iconv\_open().
&nbsp;
On calling, the *inbytesleft* parameter indicates the number of bytes in the *inbuf* buffer to be converted, and the *outbytesleft* parameter indicates the number of bytes remaining in the *outbuf* buffer that do not contain converted data. These values are updated upon return so they indicate the new state of their associated buffers.

## Returns
On success, the iconv() returns the number of non-identical conversions performed. When error occurs, the iconv() returns (size\_t) -1 and sets errno to indicate the error.

## Errors
- **EILSEQ**
Input conversion stopped due to an input byte that does not belong to the input codeset.
&nbsp;
- **E2BIG**
Input conversion stopped due to lack of space in the *outbuf*.
&nbsp;
- **EINVAL**
Input conversion stopped due to an incomplete character or shift sequence at the end of the *inbuf*.


## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
