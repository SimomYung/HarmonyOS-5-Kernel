# atexit()
Register a function that is called when the calling process exits normally

## Synopsis
```c
#include <stdlib.h>

int atexit(void (*func)(void));
```

## Arguments

- **func**
The function to be registered.

## Library
libc

## Description
The atexit() function registers a function that will be called automatically when the calling process exits normally (either by calling exit() or returns from main()). If multiple functions are registered, they will be called in the reverse order of which they are registered. At least 32 functions can be registered.
&nbsp;
When a child process is created through fork(), it inherits copies of its parent's registrations. After a successful call to any of the series of exec functions, the previous registrations are cleared.

## Returns

- **0**
Success.
&nbsp;
- **-1**
The system lacks enough memory to register a new function.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
