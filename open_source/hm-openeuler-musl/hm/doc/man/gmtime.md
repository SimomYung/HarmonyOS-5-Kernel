# gmtime(), gmtime_r(), localtime(), localtime_r()
Convert a time value into a broken-down time

## Synopsis
```c
#include <time.h>;

struct tm *gmtime(const time_t *timeptr);
struct tm *gmtime_r(const time_t *restrict timeptr,
                    struct tm *restrict result);

struct tm *localtime(const time_t *timeptr);
struct tm *localtime_r(const time_t *restrict timeptr,
                       struct tm *restrict result);
```

## Arguments
- **timeptr**
Points to the area where the time value stores.
&nbsp;
- **result**
User-supplied structure stores the conversion result broken-down time.
The tm structure is defined in &lt;time.h&gt;

## Library
libc

## Description
The gmtime() function converts the time value, which is the time in seconds since th Epoch, pointed to by *timeptr* into a broken-down time, expressed in UTC.
&nbsp;
The gmtime_r() function does the same as gmtime(), but stores the broken-down time in the structure pointed to by *result* and also returns a pointer to that same structure.
&nbsp;
The localtime() function converts the time value, which is the time in seconds since the Epoch, pointed to by  *timeptr*  into a broken-down time, expressed relative to the user's specified timezone.
&nbsp;
The localtime_r() function does the same as localtime(), but stores the broken-down time in the structure pointed to by *result* and also returns a pointer to that same structure.

## Returns
Upon successful completion, gmtime() returns a pointer to the broken-down time structure. When an error is encountered, a NULL pointer shall be returned.
&nbsp;
Upon successful completion, gmtime_r() returns a pointer to the structure pointed to by *result*.  When an error is encountered, a NULL pointer shall be returned.
&nbsp;
Upon successful completion, localtime() returns a pointer to the broken-down time structure. When an error is encountered, a NULL pointer shall be returned.
&nbsp;
Upon successful completion, localtime_r() returns a pointer to the structure pointed to by *result*.  When an error is encountered, a NULL pointer shall be returned.

## Errors
- **EOVERFLOW**
The result cannot be represented.

## Example
```c
#include <stdio.h>
#include <time.h>

int main(void)
{
	time_t result;

	result = time(NULL);
	printf("%s%ju secs since the Epoch\n",
		   asctime(localtime(&result)),
		   (uintmax_t)result);
	return 0;
}
```
This example writes the current time to stdout in a form like this:
Tue Feb 21 20:19:04 2012
1329855544 secs since the Epoch

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
