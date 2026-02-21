# getopt()
Interpret the command-line options

## Synopsis
```c
#include <unistd.h>

int getopt(int argc, char * const argv[], const char *optstring);
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

## Library
libc

## Description
Function getopt() interprets the command line (generally with '-' or '--' options). The parameter *optstring* identifies the corresponding options in the command-line.

## Returns

- **char**
Successfully find the option.
&nbsp;
- **?**
Can not find the specified option(s) (listed in *optstring*) in the command-line.
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
