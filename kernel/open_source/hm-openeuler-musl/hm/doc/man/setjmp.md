# setjmp()
Set jump point for a non-local goto

## Synopsis
```c
#include <setjmp.h>

int setjmp(jmp_buf env);
```

## Arguments

- **env**
The struct that saves the calling environment.

## Library
libc

## Description

The setjmp() function saves the calling environment in *env* for longjmp() to use. An application should ensure that an invocation of setjmp() appears in one of contexts described as follows:

* The entire controlling expression of a selection or iteration statement.

* One operand of a relational or equality operator with the other operand an integral constant expression, with the resulting expression being the entire controlling expression of a selection or iteration statement.

* The operand of a unary '!' operator with the resulting expression being the entire controlling expression of a selection or iteration.

* The entire expression of an expression statement.

## Returns

- **0**
The return is from a direct invocation.
&nbsp;

- **non-zero value**
The return is from longjmp().

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
