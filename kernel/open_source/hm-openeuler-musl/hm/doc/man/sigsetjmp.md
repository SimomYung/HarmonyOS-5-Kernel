# sigsetjmp()
Set jump point for a non-local goto

## Synopsis
```c
#include <setjmp.h>

int sigsetjmp(sigjmp_buf env, int savesigs);
```

## Arguments

- **env**
The struct that saves the calling environment.
&nbsp;
- **savesigs**
The flag that controls whether to save and restore signal mask.

## Library
libc

## Description

The sigsetjmp() function behaves the same as the setjmp() function, while its counterpart is siglongjmp() instead of longjmp(), except when *savesigs* is not 0, it also saves the current signal mask of the calling thread and will restore the saved signal mask when returning from siglongjmp().

## Returns

- **0**
The return is from a direct invocation.
&nbsp;

- **non-zero value**
The return is from siglongjmp().

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
