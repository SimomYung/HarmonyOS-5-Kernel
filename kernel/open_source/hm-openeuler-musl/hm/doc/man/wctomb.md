# wctomb()
Convert a wide character into a multibyte character

## Synopsis
```c
#include <stdlib.h>
int wctomb(char * s, wchar_t wc);
```

## Arguments
- **s**
NULL, or a pointer to a location where the function can store the multibyte character.
&nbsp;
- **wc**
The wide character to be converted.

## Library
libc

## Description
The wctomb() function determines the number of bytes required to represent the multibyte character corresponding to the code contained in *wc*. If *s* isn't NULL, the multibyte character representation is stored in the array it points to. At most MB\_CUR\_MAX characters are stored.

## Returns
If *s* is not NULL, the wctomb() function returns the number of bytes that have been written to the byte array at *s*. If *wc* can not be represented as a multibyte sequence (according to the current locale), -1 is returned.

If *s* is NULL, the wctomb() function returns nonzero if the encoding has nontrivial shift state, or zero if the encoding is stateless.

## Errors
N/A

## Examples
```c
#include <stdio.h>
#include <stdlib.h>

wchar_t wchar = {0x0073};
char mbbuffer[MB_CUR_MAX];

int main(void)
{
    int len;

    printf("Character encodings do %shave "
        "state-dependent \nencoding.\n",
        (wctomb(NULL, 0))
        ? "" : "not ");

    len = wctomb(mbbuffer, wchar);
    mbbuffer[len] = '\0';
    printf("%s(%d)\n", mbbuffer, len);
    return EXIT_SUCCESS;
}
```
This produces the output:
```c
Character encodings do not have state-dependent
encoding.
s(1)
```

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
