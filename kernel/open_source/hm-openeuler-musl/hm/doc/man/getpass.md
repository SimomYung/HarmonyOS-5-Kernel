# getpass()
Prompt for and read a password

## Synopsis
```c
#include <unistd.h>

char *getpass(const char *prompt);
```
## Arguments
- **prompt**
The string you want to display to prompt for password.

## Library
libc

## Description
The getpass() function can be used to get a password. It opens current terminal, displays the given *prompt*, suppresses echoing, reads up to 128 characters into a static buffer, and restores echoing. This function adds a null character to end of the string, but ignores additional characters and newline character.

## Returns
A pointer to static buffer.

## Errors
- **ENODEV**
The process does not have a controlling terminal.
&nbsp;
- **EBADF**
The fd of stdin is not a valid file descriptor.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
