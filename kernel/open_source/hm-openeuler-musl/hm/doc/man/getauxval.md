# getauxval()
Obtain the auxiliary information

## Synopsis
```c
#include <sys/auxv.h>

unsigned long getauxval(unsigned long item);
```

## Arguments

- **item**
*item* indicates which type of auxiliary information will be obtained.


## Library
libc

## Description
Function getauxval() gets some auxiliary information from user space program, through the ELF binary loader tool.

## Returns

- **Integer**
The value relates to the input *item*.
&nbsp;
- **0**
Failed to conduct this function.

## Errors
- **ENOENT**
When the return value is 0.


## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
