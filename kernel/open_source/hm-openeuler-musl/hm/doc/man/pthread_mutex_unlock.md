# pthread\_mutex\_unlock()
Unlock a mutex

## Synopsis
```c
#include <pthread.h>

int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

## Arguments

- **mutex**
The pointer to the mutex.

## Library
libc

## Description
The behavior of Unlock When Not Owner is described in the following table. Unlock When Not Owner means that a thread tries to unlock a mutex which is not locked by him or is not in locked state.
&nbsp;

| Mutex type | robust | Unlock When Not Owner |
|------------|--------|-----------------------|
| NORMAL/DEFAULT | N | undefined |
| NORMAL/DEFAULT | Y | return error |
| ERRORCHECK | NA | return error |
| RECURSIVE | NA | return error |

&nbsp;
The pthread\_mutex\_unlock() function will decrease the counter by one for PTHREAD\_MUTEX\_RECURSIVE mutex, or unlock the mutex for the other type mutex. The PTHREAD\_MUTEX\_RECURSIVE mutex will be unlocked when the counter reaches zero. If there are blocked threads waiting for this mutex, the scheduling policy will determine which one to be chosen to acquire the mutex after the mutex get available.
&nbsp;
Calling pthread\_mutex\_unlock() if the mutex is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EPERM**
The mutex type is PTHREAD\_MUTEX\_ERRORCHECK or PTHREAD\_MUTEX\_RECURSIVE, or the mutex is a robust mutex, and the calling thread is not the mutex owner.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
