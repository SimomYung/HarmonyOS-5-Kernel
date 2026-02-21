# wcstombs()
Convert a wide-character string into a multibyte character string

## Synopsis
```c
#include <stdlib.h>

size_t wcstombs(char* s, const wchar_t* pwcs, size_t n);
```

## Arguments
- **s**
A pointer to a buffer where the function can store the multibyte-character string.
&nbsp;
- **pwcs**
The wide-character string that you want to be converted.
&nbsp;
- **n**
The maximum number of bytes to be stored.

## Library
libc

## Description
The wcstombs() function converts a sequence of wide character codes from the array pointed to by *pwcs* into a sequence of multibyte characters, and stores them in the array pointed to by *s*. It stops if a multibyte character exceeds the limit of *n* total bytes, or if the NUL character is stored. At most *n* bytes of the array pointed to by *s* are modified.

## Returns
The number of array elements modified, not including the terminating zero code, if present, or (size\_t)-1 if an invalid multibyte character is encountered.

## Errors
N/A

## Examples
```c
#include <stdio.h>
#include <stdlib.h>

wchar_t wbuffer[] = {
    0x0073,
    0x0074,
    0x0072,
    0x0069,
    0x006e,
    0x0067,
    0x0000
};

int main(void)
{
    char mbsbuffer[50];
    int i, len;

    len = wcstombs(mbsbuffer, wbuffer, 50);
    if(len != -1) {
        for(i = 0; i < len; i++)
            printf("/%4.4x", wbuffer[i]);
        printf("\n");
        mbsbuffer[len] = '\0';
        printf("%s(%d)\n", mbsbuffer, len);
    }
    return EXIT_SUCCESS;
}
```
produces the output:
```c
/0073/0074/0072/0069/006e/0067
string(6)
```

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
