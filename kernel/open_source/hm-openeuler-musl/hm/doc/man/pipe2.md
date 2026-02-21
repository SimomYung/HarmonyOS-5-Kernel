# pipe2()
Create a pipe for interprocess communication

## Synopsis
```c
#include <fcntl.h>
#include <unistd.h>

int pipe2(int pipefd[2], int flags);
```

## Arguments
- **pipefd**
The argument *pipefd* is used to store two file descriptors, *pipefd[0]* refers to the read end of the pipe and *pipefd[1]* refers to the write end of the pipe.
&nbsp;
- **flags**
If the argument *flags* is 0 *pipe2()* is the same as *pipe()*. If *flags* is set to be the following values, different behaviors shall be obtained:
&nbsp;
  - **O_CLOEXEC**
Set the close-on-exec (FD_CLOEXEC) flag on the two new file descriptors.  See the description of the same flag in open() for reasons why this may be useful.
&nbsp;
  - **O_NONBLOCK**
Set the O_NONBLOCK file status flag on the open file descriptions referred to by the new file descriptors. Using this flag saves extra calls to fcntl() to achieve the same result.

## Library
libc

## Description
The pipe2() function creates a pipe (a unidirectional data channel) which is used for interprocess communication.

## Returns
- **0**
Create a pipe successfully, 0 is returned.
&nbsp;
- **-1**
An error occurs, -1 is returned and *errno* is set.

## Errors
- **EFAULT**
The *pipefd* is not valid.
&nbsp;
- **EINVAL**
The argument *flags* is invalid.
- **EMFILE**
All or all but one of the file descriptor available to the process are currently open.
&nbsp;
- **ENFILE**
Too many files opened in the system that exceeds the limit of the system.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
