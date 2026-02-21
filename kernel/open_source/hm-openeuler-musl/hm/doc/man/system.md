# system()
Execute a shell command

## Synopsis
```c
#include <stdlib.h>

int system(const char *command);
```

## Arguments

- **command**
The shell command to be executed.

## Library
libc

## Description

The system() library function uses posix\_spawn to create a child process that executes the busybox command specified in command using posix\_spawn as follows:

```c
posix_spawn(&pid, "${TOOL}", 0, &attr, (char *[]){"${TOOL}",
            (char *)cmd, 0}, __environ);
```
In which *${TOOL}* is the tool software like busybox, etc.

As  vfork is not supported, fork is used in in posix\_spawn (see posix\_spawn.md). This leads some different behaviours between linux when some commands executed through system().
For  example, system("ipcs -t") may lead detach time updates while it will not be updated in linux environment.

## Returns

If *command* is NULL, then a nonzero value if a shell is available, or 0 if no shell is available.
&nbsp;

If a child process could not be created, or its status could not be retrieved, the return value is -1 and errno is set to indicate the error.
&nbsp;

If a shell could not be executed in the child process, then the return value is as though the child shell terminated by calling \_exit() with the status 127.
&nbsp;

If all system calls succeed, then the return value is the termination status of the child shell used to execute command. (The termination status of a shell is the termination status of the last command it executes).

## Errors

system() can fail with any errors that could occur in posix\_spawn().

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
