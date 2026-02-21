# posix\_spawn()
Spawn a process

## Synopsis
```c
#include <spawn.h>

int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);
```

## Arguments

- **pid**
A pointer used to return the process ID of the child process.
&nbsp;
- **path**
The pathname of the file to be executed in the new process image.
&nbsp;
- **file_actions**
A pointer points to file actions to be executed between the fork step and theexec step of posix\_spawn().
&nbsp;
- **attrp**
A pointer points to attributes to be set in the child process.
&nbsp;
- **argv**
The list array of arguments for the new process image.
&nbsp;
- **envp**
The environment for the new process image.

## Library
libc

## Description

The posix\_spawn() function is used to create a new child process that executes a specified file, that is specified as a pathname.
&nbsp;

+ The posix\_spawn() function is a combination of fork() and exec(). The function can be described with three steps: the fork() step, the pre-exec() step and the exec() step.
&nbsp;

+ **fork() step**
As vfork() is not supported, fork() is used in this step, no matter what arguments *attrp* and *file_actions* is set. The process ID of the child process is placed in the argument *pid* and the control is returned to the parent process.
&nbsp;

+ **pre-exec() step**
	Before exec(), posix\_spawn() will execute a series of operations defined by the arguments *attrp* and *file_actions*, which are called housekeeping actions:

	1. Process attribute actions: Change signal masks, signal default handlers, process group, and effective user and group IDs to what are specified in *attrp*.

	2. Do file operations defined in the argument *file\_actions*.

	3. Close file descriptors with the FD\_CLOEXEC flag.
&nbsp;

	The process attributes actions are defined in *attrp*, which can be described as follows:

	1. **POSIX_SPAWN_SETSIGMASK**
Set the signal mask as the signal set specified in *attrp*. If this flag is not set, the child process will inherit signal mask from the parent process.

	2. **POSIX_SPAWN_SETSIGDEF**
Reset signal default handlers in the set specified in *spawn-sigdefault* attribute by *attrp*.

	3. **POSIX_SPAWN_RESETIDS**
Reset *UID* and *GID* of the child process to the real *UID* and *GID* of the parent process. If this flag is not set, the child process will retain the effective ones.of the parent process.

	4. **POSIX_SPAWN_SETPGROUP**
Set the process group of the child process to the group specified in *spawn-pgroup* in *attrp*. If *spawn-pgroup* is zero, the pgid of the child process will be set to pid of itself. If this flag is not set, the child process will inherit the group ID of the parent process.
&nbsp;

	The argument *file\_actions* contains a series of requests to **open, close** and **dup** files.

+ Any of operations above fails will lead to the exit of the child process with exit code 127.
&nbsp;

+ **exec() step**
	Run the requested executable file with **execve**.
&nbsp;

## Returns

- **0**
posix\_spawn() succeeds.
&nbsp;

- **error number**
An error occurred. It can be any errors that could occur in fork and exec.

## Errors
See fork() and exec().

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
