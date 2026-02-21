# cfgetospeed()
Get output baud rate

## Synopsis
```c
#include <termios.h>

speet_t cfgetospeed(const struct termios *tio);
```

## Arguments

- **tio**
A pointer to the termios structure to extract the output baud rate.


## Library
libc

## Description
The cfgetospeed() function extract the output baud rate from the termios structure to which the *tio* argument points.

## Returns

Upon successful completion, cfgetospeed() return a value of type speed\_t representing the output baud rate.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
