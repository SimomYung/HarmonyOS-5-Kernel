# environ
User environment

## Synopsis
```c
extern char **environ;
```

## Arguments
N/A

## Library
libc

## Description
*environ* is a global variable points to an array of pointers to strings called the "environment". The last pointer in this array is NULL.
&nbsp;
By convention the strings in *environ* have the form *name=value*. Common examples are:
&nbsp;

- **PATH**
The sequence of directory prefixes that sh and many other programs apply in searching for a file known by an incomplete pathname. The prefixes are separated by ':'.
- **PWD**
The current working directory.
- **SHELL**
The path of the user's login shell.
&nbsp;

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
