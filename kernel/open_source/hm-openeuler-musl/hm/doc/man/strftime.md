# strftime()
Convert a broken-down time into a string

## Synopsis
```c
#include <time.h>

size_t strftime(char *restrict s, size_t maxsize,
				const char *restrict format,const struct tm *restrict timeptr);
```

## Arguments
- **s**
Points to the area where the conversion result string stores.
&nbsp;
- **maxsize**
The size of the character array *s*.
&nbsp;
- **format**
Points to the area where the conversion specifications string stores.
&nbsp;
- **timeptr**
Points to the structure where the broken-down time stores.

## Library
libc

## Description
The strftime() function converts a broken-down time into a string as the format specification *format* and stores the result in the character array *s* of size *maxsize*.
&nbsp;
The *format* argument stores the conversion specifications, each of which is introduced by a '%' character and terminated by some other character known as a conversion specifier character. All other character sequences are ordinary character sequences. The characters of conversion specifications are replaced as follows:
- **%a**
The abbreviated name of the day of the week according to the current locale.
&nbsp;
- **%A**
The full name of the day of the week according to the current locale.
&nbsp;
- **%b**
The abbreviated month name according to the current locale.
&nbsp;
- **%B**
The full month name according to the current locale.
&nbsp;
- **%c**
The preferred date and time representation for the current locale.
&nbsp;
- **%C**
The century number (year/100) as a 2-digit integer.
&nbsp;
- **%d**
The day of the month as a decimal number (range 01 to 31).
&nbsp;
- **%D**
Equivalent to %m/%d/%y.
&nbsp;
- **%e**
Like %d, the day of the month as a decimal number, but a leading zero is replaced by a space.
&nbsp;
- **%F**
Equivalent to %Y-%m-%d.
&nbsp;
- **%G**
The ISO 8601 week-based year with century as a decimal number. The 4-digit year corresponding to the ISO week number. This has the same format and value as %Y, except that if the ISO week number belongs to the previous or next year, that year is used instead.
&nbsp;
- **%g**
Like %G, but without century, that is, with a 2-digit year (00-99).
&nbsp;
- **%h**
Equivalent to %b.
&nbsp;
- **%H**
The hour as a decimal number using a 24-hour clock (range 00 to 23).
&nbsp;
- **%I**
The hour as a decimal number using a 12-hour clock (range 01 to 12).
&nbsp;
- **%j**
The day of the year as a decimal number (range 001 to 366).
&nbsp;
- **%k**
The hour (24-hour clock) as a decimal number (range 0 to 23); single digits are preceded by a blank.
&nbsp;
- **%l**
The hour (12-hour clock) as a decimal number (range 1 to 12); single digits are preceded by a blank.
&nbsp;
- **%m**
The month as a decimal number (range 01 to 12).
&nbsp;
- **%M**
The minute as a decimal number (range 00 to 59).
&nbsp;
- **%n**
A newline character.
&nbsp;
- **%p**
Either "AM" or "PM" according to the given time value, or the corresponding strings for the current locale. Noon is treated as "PM" and midnight as "AM".
&nbsp;
- **%P**
Like %p but in lowercase: "am" or "pm" or a corresponding string for the current locale.
&nbsp;
- **%r**
The time in a.m. or p.m. notation. In the POSIX locale this is equivalent to %I:%M:%S %p.
&nbsp;
- **%R**
The time in 24-hour notation (%H:%M).
&nbsp;
- **%s**
The number of seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
&nbsp;
- **%S**
The second as a decimal number (range 00 to 60). (The range is up to 60 to allow for occasional leap seconds.)
&nbsp;
- **%t**
A tab character.
&nbsp;
- **%T**
The time in 24-hour notation (%H:%M:%S).
&nbsp;
- **%u**
The day of the week as a decimal, range 1 to 7, Monday being 1. See also %w.
&nbsp;
- **%U**
The week number of the current year as a decimal number, range 00 to 53, starting with the first Sunday as the first day of week 01.
&nbsp;
- **%V**
The ISO 8601 week number (see NOTES) of the current year as a decimal number, range 01 to 53, where week 1 is the first week that has at least 4 days in the new year.
&nbsp;
- **%w**
The day of the week as a decimal, range 0 to 6, Sunday being 0.
&nbsp;
- **%W**
The week number of the current year as a decimal number, range 00 to 53, starting with the first Monday as the first day of week 01.
&nbsp;
- **%x**
The preferred date representation for the current locale without the time.
&nbsp;
- **%X**
The preferred time representation for the current locale without the date.
&nbsp;
- **%y**
The year as a decimal number without a century (range 00 to 99).
&nbsp;
- **%Y**
The year as a decimal number including the century.
&nbsp;
- **%z**
The +hhmm or -hhmm numeric timezone (that is, the hour and minute offset from UTC).
&nbsp;
-**%Z**
The timezone name or abbreviation.
&nbsp;
-**%%**
A literal '%' character.
&nbsp;
Other conversion specifications are not supported.

## Returns
The strftime() function returns the number of bytes (excluding the terminating null byte) placed in the array *s* when the result string does not exceed *maxsize* bytes. If the length of the result string exceeds *maxsize* bytes, then strftime() returns 0, and the contents of the array are undefined.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
