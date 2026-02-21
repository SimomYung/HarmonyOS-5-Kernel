# openlog()
Send the open instruction to system logger

## Synopsis
```c
#include <syslog.h>

void openlog(const char *ident, int opt, int facility);
```

## Arguments

- **ident**
The name of the program.
&nbsp;
- **opt**
Different type of options relate to the open operation, such as:
LOG\_NDELAY, open the connection right away.
LOG\_PID, include the PID within every message.
LOG\_USER, the default option, generic user-level message.
LOG\_CONS, write messages to system console when error happened during system logger sending.
LOG\_PERROR, print stderr message.

&nbsp;
- **facility**
The *facility* provides details about what type of program is logging the information.

## Library
libc

## Description
Function openlog() opens the log operation for the given program (indicated by *ident*).


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
