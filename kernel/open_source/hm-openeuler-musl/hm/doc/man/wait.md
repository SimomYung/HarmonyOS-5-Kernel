# wait(), waitpid(), waitid(), wait4()
Wait for a process to change state

## Synopsis
```c
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *wstatus);

pid_t waitpid(pid_t pid, int *wstatus, int options);

int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

pid_t wait4(pid_t pid, int *wstatus, int options,
	    struct rusage *rusage);
```

## Arguments
- **pid**
The process ID of the process to be waited.
&nbsp;
- **wstatus**
The pointer points to an integer which stores the status information of process waited.
&nbsp;
- **options**
Options that specify the child state changes.
&nbsp;
- **rusage**
The pointer points to the struct which stores the accounting information of the child process. This feature is not supported now, so *rusage* is unused.
&nbsp;
- **idtype**
The type is used to specify which child(ren) to wait for.
&nbsp;
- **id**
The *id* and *idtype* arguments select the child(ren) to wait for.
&nbsp;
- **infop**
It records the current state of a child state changes.

## Library
libc

## Description

The wait series of functions are used to wait for state changes of a child process of the calling process. These functions can obtain information of the child process waited. A state change only has one condition that the child process terminated until now. If a terminated child process is not waited for, it will remain ZOMBIE state.
&nbsp;

The wait() function will suspend the calling thread until one of its child processes has changed state. Call wait(&status) has the same effect as:

```c
waitpid(-1, &wstatus, 0);
```
As the argument *rusage* is not supported, wait4() is exactly the same as waitpid().
&nbsp;
The value of *pid* can be:

- **-1**
Wait for any child process.
&nbsp;
- **\>0**
Wait for the child whose process ID is equal to the value of pid.
&nbsp;
- **0**
Wait for any child process whose process group ID is equal to that of the calling process.
&nbsp;
- **\<-1**
Wait for any child process whose process group ID is equal to the absolute value of pid.
&nbsp;
The value of *idtype* can be:

- **P_PID**
Wait for the child whose process ID matches id.
&nbsp;
- **P_PGID**
Wait for any child whose process group ID matches id.
&nbsp;
- **P_ALL**
Wait for any child; id is ignored.
&nbsp;
The value of options is an OR of zero or more of the following constants:

- **WNOHANG**
Return immediately if no child has exited.
&nbsp;
- **WEXITED**
Wait for children that have terminated.
&nbsp;
- **WUNTRACED**
Return if a child has stopped, otherwise wait.
&nbsp;
- **WCONTINUED**
Return if a stopped child has been resumed by a SIGCONT signal.
&nbsp;
- **__WNOTHREAD**
Only wait children in the current thread, not including other threads in the same thread group.
&nbsp;
- **__WCLONE**
Wait for "clone" children, a "clone" child is one which sends no signal or a signal other than
SIGCHLD to its parent when it's terminated. This option is ignored if **__WALL** is also specified.
&nbsp;
- **__WALL**
Wait for all children, including "clone" children and "non-clone" children.
&nbsp;
As the function of system calls restart is not support currently, these interfaces will be interrupted and return EINTR when get signals which have already set the flag SA\_RESTART by sigaction. If want continue to wait for child process after get signals, use loop to try again is a feasible method.
&nbsp;
The integer that *wstatus* points to can be inspected with the following macros:
&nbsp;

- **WIFEXITED(wstatus)**
Returns true if the child terminated normally, that is, by calling exit() or _exit(), or by returning from main().

- **WEXITSTATUS(wstatus)**
Returns the exit status of the child.

- **WIFSIGNALED(wstatus)**
Returns true if the child process was terminated by a signal.

- **WTERMSIG(wstatus)**
Returns the signal number that caused the child process to terminate.This macro should be employed only if WIFSIGNALED returns true.

- **WCOREDUMP(wstatus)**
Returns true if the child produced a core dump. This macro should be employed only if WIFSIGNALED returns true.

- **WIFSTOPPED(wstatus)**
Returns true if the child process was stopped by a signal.

- **WIFCONTINUED(wstatus)**
Returns true if the child process was resumed by a SIGCONT signal.

- **WSTOPSIG(wstatus)**
Returns the signal number which caused the child to stop. The macro can be used only if WIFSTOPPED returned true.

```c
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
```
The waitid() interface provides more control parameters to wait for the status change of the child.  

The idtype and id parameters select which the child(ren) to wait for.  

idtype == **P_PID**  
Wait for the child that matches the id.  
idtype == **P_PGID**  
Wait for any child whose process group ID matches id.  
idtype == **P_ALL**  
Wait for any child; id is ignored.  

options == **WEXITED**  
As for waitpid().  
options == **WNOHANG**  
As for waitpid().  
options == **WNOWAIT**  
Keeps the child in a waitable state so that the parent can wait for the child process repeatedly.  
options == **WSTOPPED**
Wait for child processes that have been stopped by a signal.
options == **WCONTINUED**
Wait for (previously stopped) child processes that have been resumed signal SIGCONT.

If waitid is returned successfully, **siginfo_t** is filled.  
si_pid: The process ID of the child.  
si_uid: The real user ID of the child.  
si_signo: Always set to SIGCHLD.  
si_status: the exit status of the child.  
si_code: CLD_EXITED child called exit. CLD_DUMPED child killed by signal and dumped core. CLD_KILLED child killed by signal.  

## Returns

**wait()**
On success, returns the process ID of the terminated child; on error, -1 is returned.
&nbsp;
**waitpid()**
On success, returns the process ID of the child whose state has changed; if WNOHANG was specified but the child process waited have not yet changed state, then 0 is returned.On error, -1 is returned.
&nbsp;
**wait4()**
The same as waitpid().
&nbsp;
**waitid()**
Returns 0 on success or if WNOHANG was specified and no child(ren) specified by *id* has yet changed state; on error, -1 is returned.

## Errors

- **ECHILD**(for wait())
There is not any unwaited child process for the calling process.
&nbsp;
- **ECHILD**(for waitpid() and wait4() and waitid())
The process specified by *pid* does not exist or *idtype* and *id* does not exist or is not a child of the calling process.
&nbsp;
- **EINTR**
**WNOHANG** is not set and an unblocked siganl or SIGCHLD is caught.
&nbsp;
- **EINVAL**
The argument *options* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
