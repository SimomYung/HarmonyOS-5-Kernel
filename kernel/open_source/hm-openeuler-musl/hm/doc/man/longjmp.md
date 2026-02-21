# longjmp()
Non-local goto

## Synopsis
```c
#include <setjmp.h>

void longjmp(jmp_buf env, int val);
```

## Arguments

- **env**
The struct that stores the environment saved by the most recent invocation.
&nbsp;
- **val**
The value to be returned by setjmp().

## Library
libc

## Description

The longjmp() function will restore the environment saved by the most recent invocation of setjmp(), with the argument *env*.
&nbsp;
In following conditions, the behavior can be undefined:

1. The most recent invocation of setjmp() with *env* occurred in another thread.

2. There is no most recent invocation of setjmp() with *env*.

3. The function containing the invocation of setjmp() has terminated in interim.

4. The invocation occurred within the scope of an identifier with variably modified type and the execution has left the scope in interim.

5. The longjmp() function is invoked from a nested signal handler.

6. The longjmp() function is called with *env* uninitialized.
&nbsp;

After the longjmp() function is called, all objects that is accessible and all other components of the abstract machine have state, except that the values of objects of automatic storage duration are unspecified if the objects meet all the following conditions:

1. The values of objects are local to the function including the related setjmp() invocation.

2. The values of objects do not have volatile-qualified type.

3. The values of objects are changed between the setjmp() invocation and longjmp().

## Returns

- **val**
Program continues as if the related invocation of setjmp() returns *val*.
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
