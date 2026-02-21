# alarm()
Set an alarm clock for delivery of a signal

## Synopsis
```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```

## Arguments

- **seconds**
The number of seconds before the alarm signal delivered.

## Library
libc

## Description

The alarm() function delivers a SIGALRM signal to the calling process in *seconds* seconds. If *seconds* is zero, any pending alarm is canceled in any event any previously set.
&nbsp;
The alarm() function can only send a signal to the caller thread. When the caller thread exits before the alarm expires, the alarm will be canceled and the process cannot receive the signal.

## Returns

- **seconds remaining**
The number of seconds remaining until any previously set alarm is delivered.
&nbsp;

- **0**
There is no previously set alarm.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
