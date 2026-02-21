# asctime(), asctime_r()
Convert the broken-down time into a string

## Synopsis
```c
#include <time.h>;

char *asctime(const struct tm *timeptr);
char *asctime_r(const struct tm *restrict timeptr, char *restrict buf);
```

## Arguments
- **timeptr**
Points to the structure where the broken-down time stores.
The tm structure is defined in &lt;time.h&gt;
&nbsp;
- **buf**
User-supplied buffer which should have room for at least 26 bytes to store the conversion result string.

## Library
libc

## Description
The asctime() function converts the broken-down time into a string in the form:
\"Fri Feb 21 10:51:11 2020\n\0\"
&nbsp;
The asctime_r() function converts the broken-down time into a string, in the same form as that returned by asctime(), that is placed in the user-supplied buffer and returns the buffer pointer.

## Returns
Upon successful completion, asctime() returns a pointer to the conversion result string. When an error is encountered, a NULL pointer shall be returned.
&nbsp;
Upon successful completion, asctime_r() returns the user-supplied buffer pointer containing the conversion result string. When an error is encountered, a NULL pointer shall be returned.

## Errors
N/A

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
