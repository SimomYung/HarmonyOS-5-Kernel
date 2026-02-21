# cfsetospeed()
Set output baud rate

## Synopsis
```c
#include <termios.h>

int cfsetospeed(struct termios *tio, speed_t speed);
```

## Arguments

- **tio**
A pointer to the termios structure to set the output baud rate.
&nbsp;
- **speed**
The baud rate to be stored in the termios structure.


## Library
libc

## Description
The cfsetospeed() function set the output baud rate stored in the structure pointed to by *tio* to *speed*. There be no effect on the baud rates set in the hardware until a subsequent successful call to tcsetattr() with the same termios structure. Similarly, errors resulting from attempts to set baud rates not supported by the terminal device need not be detected until the tcsetattr() function is called.

## Returns

- **0**
The output baud rate was successfully set into the termios structure.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EINVAL**
The speed value is not a valid baud rate or outside the range of possible speed values as specified in &lt;termios.h&gt;.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
