# getopt\_long(), getopt\_long\_only()
Interpret the command-line options

## Synopsis
```c
#include <unistd.h>

int getopt_long(int argc, char *const *argv, const char *optstring, const struct option *longopts, int *idx);
int getopt_long_only(int argc, char *const *argv, const char *optstring, const struct option *longopts, int *idx);
```

## Arguments

- **argc**
The number of input parameters.
&nbsp;
- **argv[]**
The list of all the parameters in command line.
&nbsp;
- **optstring**
The defined options.
&nbsp;
- **longopts**
Pointer to the struct option, where the *option* is defined as:
```c
struct option {
        const char *name;
        int has_arg;
        int *flag;
        int val;
};
```
&nbsp;
- **idx**
If *idx* is not NULL, it means the long option's index in *longopts* parameter.

## Library
libc

## Description
Similar to getopt(), function getopt\_long() interprets the command line but accepts long options as well. (the option starts with '--' is considered as long option). Function getopt\_long\_only() is similar to getopt\_long(), but the single dash will be treated as double dashes.

## Returns

- **char**
Successfully find the short option.
&nbsp;
- **val**
Successfully find the long option, the *val* here comes from the struct option.
&nbsp;
- **?**
Can not find the specified option(s) (listed in *optstring*) in the command-line or function encounters an extraneous parameter.
&nbsp;
- **-1**
All command-line options have been interpreted.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
