# pthread\_once()
Do initialization that only needs to be executed once

## Synopsis
```c
#include <pthread.h>

int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
pthread_once_t once_control = PTHREAD_ONCE_INIT;
```

## Arguments

- **once\_control**
The parameter which determine whether *init\_routine* has been called.
&nbsp;
- **init\_routine**
The *init\_routine* associated with once\_control.

## Library
libc

## Description
The given *once\_control* can be used to call the *init\_routine* only once. The *init\_routine* will be called at the first time pthread\_once() is called by any thread in a process, and subsequent calls of pthread\_once() with the same *once\_control* will return immediately with no call of *init\_routine*. *Init\_routine* will be completed on return from pthread\_once(). If *init\_routine* has a cancellation point and is canceled, *once\_control* will be left as if pthread\_once() was never called.
&nbsp;
Using *once\_control* which has automatic storage duration or is uninitialized results in undefined behavior.

## Returns

- **0**
Success.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
