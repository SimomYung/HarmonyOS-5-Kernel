# backtrace()
Get the backtrace of the program

## Synopsis

```c
#include <execinfo.h>

int backtrace(void **buffer, int size);
```

## Arguments

- **buffer**
The array to store backtrace.
&nbsp;
- **size**
The maximum number of addresses that *buffer* can store.

## Library

libc

## Description

The backtrace() function stores the backtrace of the calling program in *buffer*.

## Returns

- **0**
An error occurred.
&nbsp;
- **positive integer**
On success, backtrace() returns the number of function addresses it acquired.

## Errors

N/A

## Example
&nbsp;

## Classification

Unix

## Function Safety

TODO
