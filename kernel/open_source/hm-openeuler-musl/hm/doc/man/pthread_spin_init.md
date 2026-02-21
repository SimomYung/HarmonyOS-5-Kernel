# pthread\_spin\_init()
Initialize a spin lock

## Synopsis
```c
#include <pthread.h>

int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
```

## Arguments

- **lock**
The pointer to the spin lock.
&nbsp;
- **pshared**
The process shared mode.

## Library
libc

## Description
The pthread\_spin\_init() function initializes the spin lock with the attribute specified by *pshared*. The spin lock will become initialized and unlocked after successful initialization. The argument *pshared* should be one of the following values:
&nbsp;
- **PTHREAD\_PROCESS\_PRIVATE**
The spin lock can only be used by threads in the same process which the calling thread in. Using this kind of spin lock between processes results in undefined behavior.
&nbsp;
- **PTHREAD\_PROCESS\_SHARED**
The spin lock can be used by any thread in any process.

&nbsp;
Using an uninitialized spin lock results in undefined behavior. Referencing a destroyed spin lock results in undefined behavior, unless it is reinitialized using pthread\_spin\_init(). Using copies of the spin lock in calls to pthread\_spin\_lock(), pthread\_spin\_unlock(), pthread\_spin\_destroy() results in undefined behavior. Initializing an already initialized spin lock results in undefined behavior.

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
