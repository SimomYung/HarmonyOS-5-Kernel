# ctime(), ctime_r()
Convert a time value into a string

## Synopsis
```c
#include <time.h>;

char *ctime(const time_t *clock);
char *ctime_r(const time_t *clock, char *buf);
```

## Arguments
- **clock**
Points to the area where the time value stores.
&nbsp;
- **buf**
User-supplied buffer which should have room for at least 26 bytes to store the conversion result string.

## Library
libc

## Description
The ctime() function converts the time value, which is the time in seconds since the Epoch, into a string. It should be equivalent to:
```c
	asctime(localtime(clock));
```
&nbsp;
The ctime_r() function converts the time value into a string in the same form as that returned by ctime() and puts the string in the user-supplied buffer and returns the buffer pointer.

## Returns
Upon successful completion, ctime() returns the pointer returned by asctime() with a broken-down time as an argument. When an error is encountered, a NULL pointer shall be returned.
&nbsp;
Upon successful completion, ctime_r() returns the user-supplied buffer pointer containing the conversion result string. When an error is encountered, a NULL pointer shall be returned.

## Errors
N/A

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
