# pthread\_cleanup\_pop()
Pop a cancellation cleanup handler

## Synopsis
```c
#include <pthread.h>

void pthread_cleanup_pop(int execute);
```

## Arguments

- **execute**
Whether the popped cancellation cleanup handler will be executed.

## Library
libc

## Description
The pthread\_cleanup\_pop() function pops a cancellation cleanup handler from the top of the calling thread's cancellation cleanup stack. If a non-zero parameter is passed as *execute*, then the popped cleanup handler will be executed.
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
