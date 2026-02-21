# syscall()
Invoke a Linux system call indirectly.

## Synopsis
```c
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

long syscall(long number, ...);

```
## Arguments
- **number**
Specify the assembly language interface of the syscall.
&nbsp;
- **...**
The specified arguments of the syscall.

## Library
libc

## Description
The syscall() function invokes the system call indirectly.

## Returns
The return value is defined by the system call being invoked.

## Errors
The errno is set by the system call being invoked.

## Example
```c
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[])
{
	pid_t tid;

	tid = syscall(SYS_gettid);
	tid = syscall(SYS_tgkill, getpid(), tid, SIGHUP);
}
```

## Classification
Unix

## Function Safety
TODO
