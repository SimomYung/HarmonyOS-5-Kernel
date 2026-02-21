# syslog()
Write a message to the system logger

## Synopsis
```c
#include <syslog.h>

void syslog(int priority, const char *message, ...);
```

## Arguments

- **priority**
The combination of *facility* and *level* values, which is from openlog().
&nbsp;
- **message**
The message can be formatted with specific operations, i.e., sprintf().

## Library
libc

## Description
Function syslog() creates a log message (which can be formatted by caller).
syslog() supports different types of options related to the open operation, such as:
LOG\_NDELAY, open the connection right away.
LOG\_PID, include the PID within every message.
LOG\_USER, the default option, generic user-level message.
LOG\_CONS, write messages to system console when error happened during system logger sending.
LOG\_PERROR, print stderr message.

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
