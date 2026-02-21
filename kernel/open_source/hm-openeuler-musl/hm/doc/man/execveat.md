# execveat()
Execute program relative to a directory file descriptor

## Synopsis
```c
#include <unistd.h>

int execveat(int dirfd, const char *pathname,
             char *const argv[], char *const envp[],
             int flags);
```

## Arguments

- **dirfd**
The file descriptor that specifies the program to be executed.
&nbsp;

- **pathname**
The path name that specifies the program to be executed.
&nbsp;

- **argv**
The list array of arguments for the new process image.
&nbsp;

- **envp**
The environment for the new process image.
&nbsp;

- **flags**
The flags that define behavior of the function.

## Library
libc

## Description

The execveat() function executes the program referred to by the combination of *dirfd* and *pathname*. It has the same behavior as execve(), except for the differences described in this doc.
&nbsp;

If *pathname* is a relative path, it will be interpreted relative to the directory referred to by the file descriptor *dirfd*. If *pathname* is an absolute path, *dirfd* will be ignored.
&nbsp;

Following *flags* are supported:

- **AT\_EMPTY\_PATH**
If *pathname* is an empty string, the file specified by *dirfd* will be executed.

- **AT\_SYMLINK\_NOFOLLOW**
If the file specified by *dirfd* and a non-NULL pathname is a symbolic link, then the execveat() fails with the error ELOOP.

## Returns

- **-1**
An error occurred.

## Errors

- **EBADF**
*dirfd* is an invalid file descriptor.
&nbsp;
- **EINVAL**
*flags* is invalid.
&nbsp;
- **ELOOP**
*flags* includes AT\_SYMLINK\_NOFOLLOW and the file specified by *dirfd* and a non-NULL pathname is a symbolic link.
&nbsp;
- **ENOTDIR**
*pathname* is a relative path and *dirfd* is a file descriptor referring to a file other than a directory.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
