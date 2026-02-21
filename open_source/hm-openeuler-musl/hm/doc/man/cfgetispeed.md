# cfgetispeed()
Get input baud rate

## Synopsis
```c
#include <termios.h>

speet_t cfgetispeed(const struct termios *tio);
```

## Arguments

- **tio**
A pointer to the termios structure to extract the input baud rate.


## Library
libc

## Description
The cfgetispeed() function extract the input baud rate from the termios structure to which the *tio* argument points.

## Returns

Upon successful completion, cfgetispeed() return a value of type speed\_t representing the input baud rate.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
