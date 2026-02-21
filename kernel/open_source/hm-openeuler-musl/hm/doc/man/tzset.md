# tzset(), tzname, timezone, daylight
Set timezone conversion information

## Synopsis
```c
#include <times.h>;

extern int daylight;
extern long timezone;
extern char *tzname[2];
void tzset(void);
```

## Arguments
N/A

## Library
libc

## Description
The tzset() function uses the value of the environment variable *TZ* to set time conversion information used by ctime(), localtime(), mktime(), and strftime(). If *TZ* is absent from the environment, implementation-defined default timezone information shell be used.

The *tzset*() function shall set the external variable *tzname* as follows:

```c
tzname[0] = "std";
tzname[1] = "dst";
```
The tzset() function also shall set the external variable *daylight* to 0 if Daylight Savings Time conversions should never be applied for the timezone in use; otherwise, non-zero. The external variable *timezone* shall be set to the difference, in seconds, between Coordinated Universal Time (UTC) and local standard time.


## Returns
N/A

## Errors
N/A

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
