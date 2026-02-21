# cfmakeraw()
Set the terminal to "raw" mode

## Synopsis
```c
#define _GNU_SOURCE
#include <termios.h>

void cfmakeraw(struct termios *tio);
```

## Arguments

- **tio**
A pointer to the termios structure to set to "raw" mode.


## Library
libc

## Description
cfmakeraw() sets the terminal to something like the "raw" mode of the old Version 7 terminal driver: input is available character by character, echoing is disabled, and all special processing of terminal input and output characters is disabled. The terminal attributes are set as follows:
```c
	tio->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	tio->c_oflag &= ~OPOST;
	tio->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	tio->c_cflag &= ~(CSIZE|PARENB);
	tio->c_cflag |= CS8;
	tio->c_cc[VMIN] = 1;
	tio->c_cc[VTIME] = 0;
```

## Returns
N/A

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
