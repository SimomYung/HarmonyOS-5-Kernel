# mbtowc()
Convert a multibyte character into a wide character

## Synopsis
```c
#include <stdlib.h>

int mbtowc(wchar_t * pwc, const char * s, size_t n);
```

## Arguments
- **pwc**
A pointer to a *wchar_t* object where the function can store the wide character.
&nbsp;
- **s**
NULL (see below), or a pointer to the multibyte character to be converted.
&nbsp;
- **n**
The maximum number of bytes in the multibyte character to be converted.

## Library
libc

## Description
The mbtowc() function converts a single multibyte character pointed to by *s* into a wide-character code pointed to by *pwc*, to a maximum of *n* bytes. The function stops early if it encounters the NULL character. This function is affected by LC\_TYPE. The mbrtowc() function is a restartable version of mbtowc().

## Returns
If *s* is NULL:
&nbsp;
- **nonzero**
The mbtowc() function returns nonzero if the encoding has nontrivial shift state.
&nbsp;
- **0**
The encoding is stateless.
&nbsp;


If *s* is not NULL:
&nbsp;
- **> 0**
The mbtowc() function returns the number of consumed bytes starting at *s*.
&nbsp;
- **0**
*s* points to the NUL character.
&nbsp;
- **-1**
Failure, The next *n* bytes don't form a valid multibyte character.

## Errors
- **EILSEQ**
Invalid character sequence.

## Examples
```c
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *wc = "string";
    wchar_t wbuffer[10];
    int i, len;

    printf("State-dependent encoding? ");
    if(mbtowc(wbuffer, NULL, 0)) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }

    len = mbtowc(wbuffer, wc, 2);
    wbuffer[len] = '\0';
    printf("%s(%d)\n", wc, len);

    for(i = 0; i < len; i++) {
        printf("/%4.4x", wbuffer[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
```
This produces the output:
```c
State-dependent encoding? No
string(1)
/0073
```

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
