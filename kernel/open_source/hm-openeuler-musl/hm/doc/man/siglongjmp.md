# siglongjmp()
Non-local goto

## Synopsis
```c
#include <setjmp.h>

void siglongjmp(sigjmp_buf env, int val);
```

## Arguments

- **env**
The struct that stores the environment saved by the most recent invocation.
&nbsp;
- **val**
The value to be returned by sigsetjmp().

## Library
libc

## Description

The siglongjmp() function behaves the same as the longjmp() function, while its counterpart is sigsetjmp() instead of setjmp().

## Returns

- **val**
Program continues as if the related invocation of sigsetjmp() returns *val*.
&nbsp;

- **1**
*val* is equal to zero.

## Errors

N/A

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
