# fnmatch()
Check whether the string argument matches the pattern argument

## Synopsis
```c
#include <fnmatch.h>;

int fnmatch(const char *pat, const char *str, int flags);
```
## Arguments

- **pat**
The pattern to match.
&nbsp;
- **str**
The string to match against the pattern.
&nbsp;
- **flags**
Indicate the behavior in process.

## Library
libc

## Description
The fnmatch() function checks whether the *str* argument matches the pattern argument which is a shell wildcard.

The flags can be bitwise OR of zero or more of the following flags:
- **FNM_NOESCAPE**
Treat backslashes as ordinary characters, not escape characters.
&nbsp;
- **FNM_PATHNAME**
match a slash in string only with a slash in pattern and not by an asterisk (*) or a question mark (?) metacharacter, nor by a bracket expression ([]) containing a slash.
&nbsp;
- **FNM_PERIOD**
The leading period in the string must exactly match the period in the pattern. If the period is the first character in the string, or if both FNM_PATHNAME are set and the period is immediately followed by a slash, it is considered a leading character.
&nbsp;
- **FNM_FILE_NAME**
This is a GNU synonym for FNM_PATHNAME.
&nbsp;
- **FNM_LEADING_DIR**
If this flag is set, the pattern is considered to be matched if it matches an initial segment of string which is followed by a slash. This flag is mainly for internal use and is only implemented in certain cases.
&nbsp;
- **FNM_CASEFOLD**
If this flag is set, the pattern is matched case-insensitively.
## Returns
Zero for success, or a *FNM_NOMATCH* value for fail.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
