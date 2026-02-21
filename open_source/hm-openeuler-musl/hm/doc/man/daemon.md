# daemon()
Run in the background

## Synopsis
```c
#include <unistd.h>

int daemon(int nochdir, int noclose);
```

## Arguments

- **nochdir**
If *nochdir* is zero, daemon() changes the calling process's current working directory to the root directory ("/"); otherwise, the current working directory is left unchanged.
&nbsp;
- **noclose**
If *noclose* is zero, daemon() redirects standard input, standard output and standard error to /dev/null; otherwise, no changes are made to these file descriptors.

## Library
libc

## Description
The daemon() function detaches the program from the controlling terminal and run in the background as system daemons.

## Returns

- **0**
daemon() executes successfully.
&nbsp;
- **-1**
An error occurred.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
