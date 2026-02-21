# pthread\_cleanup\_push()
Push a cancellation cleanup handler

## Synopsis
```c
#include <pthread.h>

void pthread_cleanup_push(void (*handler)(void *), void *arg);
```

## Arguments

- **handler**
Cleanup handler which is a function.
&nbsp;
- **arg**
Argument needed by the cleanup handler.

## Library
libc

## Description
The pthread\_cleanup\_push() function pushes the specified cancellation cleanup handler onto the calling thread's cancellation cleanup stack. When the calling thread exits (be canceled or exits normally) or the calling thread calls pthread\_cleanup\_pop() with non-zero parameter, the top-most cleanup handler will be popped and invoked with *arg* as argument.
&nbsp;
Note that pthread\_cleanup\_push() and pthread\_cleanup\_pop() functions are implemented as macros contain '{' and '}' respectively. This is to make sure that these two functions are called in pairs in the same function with the same nesting level. So be careful to use new variables between a pair of pthread\_cleanup\_push() and pthread\_cleanup\_pop() because they are visible only inside the paired calls.

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
