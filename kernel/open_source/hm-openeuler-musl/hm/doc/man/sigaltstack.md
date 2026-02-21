# sigaltstack()
Set and/or get signal stack context

## Synopsis
```c
#include <signal.h>

int sigaltstack(const stack_t *__restrict ss, stack_t *__restrict old_ss);
```

## Arguments

- **ss**
The new alternate signal stack.
&nbsp;
- **old\_ss**
The currently established signal stack.

## Library
libc

## Description
The sigaltstack() function allows a process to define a new alternate signal stack and/or retrieve the state of an existing alternate signal stack. An alternate signal stack is used during the execution of a signal handler if the establishment of that handler requested it.
&nbsp;
While a program is running on the alternated stack (during a signal handler), an error will occur if another signal which also uses the alternated stack arrives. The stack will be overlapped in this situation.
&nbsp;
The *stack\_t* type is defined as follows:
```c
typedef struct {
	void  *ss_sp;     /* Base address of stack */
	int    ss_flags;  /* Flags */
	size_t ss_size;   /* Number of bytes in stack */
} stack_t;
```
&nbsp;
Only supports to set 0 or SS\_DISABLE to the *ss\_flags*.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
Either *ss* or *old\_ss* is not NULL and points to an address outside of the process's address space.
&nbsp;
- **EINVAL**
*ss* is not NULL and the *ss\_flags* field contains an invalid flag.
&nbsp;
- **ENOMEM**
*ss.ss\_size* was less than MINSIGSTKSZ.
&nbsp;
- **EPERM**
An attempt was made to change the alternate signal stack while it was active.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
