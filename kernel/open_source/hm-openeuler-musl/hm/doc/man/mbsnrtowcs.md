# mbsnrtowcs()
Convert a multibyte string to a wide-character string

## Synopsis
```c
#include <wchar.h>

size_t mbsnrtowcs(wchar_t *dest, const char **src,
                  size_t nms, size_t len, mbstate_t *ps);
```

## Arguments
- **dest**
A pointer to a buffer where the function can store the wide-character string.
&nbsp;
- **src**
The string of multibyte characters to be converted.
&nbsp;
- **nms**
The maximum number of bytes to be converted, starting at *\*src*.
&nbsp;
- **len**
The maximum number of wide characters can be written to *dest*.
&nbsp;
- **ps**
An internal pointer that lets mbsrtowcs() be a restartable version of mbstowcs(); if *ps* is NULL, mbsrtowcs() uses its own internal variable.

## Library
libc

## Description
The mbsnrtowcs() function converts a string of multibyte characters pointed to by *src* into the corresponding wide characters pointed to by *dst*, to a maximum of *n* wide characters, including the terminating NULL character, and the number of bytes to be converted, starting at *\*src*, is limited to *nms*.
&nbsp;
       If *dest* is not NULL, the mbsnrtowcs() function converts at most *nms* bytes from the multibyte string *\*src* to a wide-character string starting at *dest*. At most *len* wide characters are written to *dest*. The shift state *\*ps* is updated. The conversion is effectively performed by repeatedly calling mbrtowc(*dest*, *\*src*, *n*, *ps*) where *n* is some positive number, as long as this call succeeds, and then incrementing dest by one and *\*src* by the number of bytes consumed. The conversion can stop for three reasons:
&nbsp;
       1. An invalid multibyte sequence has been encountered. In this case, *\*src* is left pointing to the invalid multibyte sequence, (size_t) -1 is returned, and errno is set to EILSEQ.
&nbsp;
       2. The *nms* limit forces a stop, or *len* non-L'\0' wide characters have been stored at *dest*. In this case, *\*src* is left pointing to the next multibyte sequence to be converted, and the number of wide characters written to *dest* is returned.
&nbsp;
       3. The multibyte string has been completely converted, including the terminating NULL wide character ('\0') (which has the side effect of bringing back *\*ps* to the initial state). In this case, *\*src* is set to NULL, and the number of wide characters written to *dest*, excluding the terminating NULL wide character, is returned.
&nbsp;
       If *dest* is NULL, *len* is ignored, and the conversion proceeds as above, except that the converted wide characters are not written out to memory, and that no destination length limit exists.
&nbsp;
       In both of the above cases, if *ps* is NULL, a static anonymous state known only to the mbsnrtowcs() function is used instead.
&nbsp;
       The programmer must ensure that there is room for at least *len* wide characters at *dest*.

## Returns
The mbsnrtowcs() function returns the number of wide characters that make up the converted part of the wide-character string, not including the terminating NULL wide character. If an invalid multibyte sequence was encountered, (size_t) -1 is returned, and errno set to EILSEQ.

## Errors
- **EILSEQ**
An invalid multibyte sequence is encountered.

## Examples
&nbsp;

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
