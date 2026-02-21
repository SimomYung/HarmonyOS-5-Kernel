# btowc()
Convert a single-byte character to a wide character

## Synopsis
```c
#include <wchar.h>

wint_t btowc(int c);

```

## Arguments
- **c**
The single-byte character to be converted.

## Library
libc

## Description
The btowc() function converts the given character (if it's a valid one-byte character in the initial shift state) into a wide character.

## Returns
The wide-character representation of the character, or WEOF if *c* has the value EOF or (unsigned char) *c* isn't a valid one-byte character in the initial conversion state.

## Errors
N/A

## Examples
&nbsp;

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
