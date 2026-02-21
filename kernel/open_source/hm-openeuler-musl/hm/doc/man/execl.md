# execl(), execle(), execlp(), execv(), execve(), execvp(), execvpe(), fexecve()
Execute a file

## Synopsis
```c
#include <unisted.h>;

int execl(const char *path, const char *arg0, ... /*, (char *)0 */);

int execle(const char *path, const char *arg0, ... /*,(char *)0, char *const envp[]*/);

int execlp(const char *file, const char *arg0, ... /*, (char *)0 */);

int execv(const char *path, char *const argv[]);

int execve(const char *path, char *const argv[], char *const envp[]);

int execvp(const char *file, char *const argv[]);

int execvpe(const char *file, char *const argv[], char *const envp[]);

int fexecve(int fd, char *const argv[], char *const envp[]);
```
## Arguments

- **path**
The path to the file that you want to execute.
&nbsp;
- **arg0, ... **
The list of arguments for the new process image.
&nbsp;
- **file**
Filename of the file that you want to execute.
&nbsp;
- **argv**
The list array of arguments for the new process image.
&nbsp;
- **envp**
The environment for the new process image.
&nbsp;
- **fd**
The file descriptor of the file that you want to execute.
&nbsp;

## Library
libc

## Description

The series of functions in exec family exexute a executable file, which is called a new process image. The new process image will replace the old one. Nothing will be returned when exec succeeds, as the calling process image is overlaid by the new one. *path*(or *file*) must be either an elf-format executable file, or a script file. The function fexecve() executes file specified by *fd*, while other functions execute file with argument *path* or *file*.
&nbsp;

Executing a C-language program by exec functions is the same as calling:
```
int main(int argc, char *argv[]);
```
in which *argc* is the count of arguments and *argv* is arguments array. And the arguments will be passed on to the new process image.
&nbsp;

The arguments *arg0, ...* represent a series of pointers to NULL-terminated strings. These strings will form a list of parameters available for the new process image. The list should end with a NULL pointer. Parameter *arg0* should point to the filename string associated with the process being started with one of the exec functions.

The argument *argv* is an array of character pointers to NULL-terminated strings. The application should ensure that the last member of the array is a NULL pointer. These strings will form a list of parameters available for the new process image. Value in *argv[0]* should point to the filename string associated with the process being started with one of the exec functions.

The argument *envp* is an array of character pointers to NULL-terminated strings. These strings should form the environment for new process images. The *envp* array should end with a NULL pointer.

The arguments *argv* and *envp* can't contain NULL string or NULL character in the middle. **execve** *envp* parameter supports NULL.

The argument *path* must be specified pointing to '/pathname/filename' now, meaning that *path* must be an absolute path and relative path like './filename' is not supported. Only 'filename' is not supported.

The argument *file* is used to construct a path name of new image. If there is no slash('/') in *file*, the path prefix will be searched in the environment variable *PATH*. Otherwise, the argument *file* will be used as executable file path directly.
&nbsp;

Bytes of argument and environment lists, include the NULL terminators and NULL pointers, is limited to {ARG\_MAX}, which is 32 pages.

File descriptors except network fd and shared memory fd open in the calling process image will remain open in the new process image, except for those whose close-on-exec flag FD\_CLOEXEC is set. Network fd and shared memory fd are not currently supported but will be supported in the future. File descriptors 0, 1, or 2 will be opened at the beginning of the new process image.

Signals set to be ignored by the calling process image will be set to be ignored by the new process image. Other actions will be set to the default action. After a successful call alternate signal stacks are not preserved and
the SA\_ONSTACK flag will be cleared for all signals.
&nbsp;

A call to any exec function from a process with more than one thread is supported. Other features are not supported yet.
&nbsp;

If *path*(or *file*) is a script, the first line of the file will be extracted, which is of the form: \#\!*interpreter optional-arg*. *interpreter* is the script interpreter and must be a valid pathname for an executable file. It is not supported if *interpreter* is also a script. *optional-arg* is either absent or the entire string following *interpreter*(even if it contains white space). The exec functions will execute the interpreter program with the following arguments:
&nbsp;
*interpreter optional-arg pathname arg...*
&nbsp;
*pathname* is the absolute path of the script file, and *arg...* is the series of string pointed to by *argv* starting at *argv[1]*. If the format of *path*(or *file*) is not recognized, the exec functions will fail with ENOEXEC. Because of the lack of the interpreter program, only bash script file is supported.

## Returns

- **-1**
An error occurred.

## Errors

The exec functions fail if:

- **E2BIG**
The number of bytes of argument list and environment list is greater than {ARG\_MAX}.
&nbsp;
- **EACCES**
The new process image file is not a regular file and its type is unsupported.
&nbsp;
- **EINVAL**
The new process image file has appropriate privileges and has a recognized executable binary format, but executing a file with this format is not supported by system.
&nbsp;
- **EFAULT**
Filename or a pointer in the vectors *argv* or *envp* points outside your accessible address space.
&nbsp;

The exec functions, except for fexecve(), fail if:

- **EACCES**
It is not allowed to search for a directory listed in the new process image file's path prefix, or the new process image file denies execution permission. The execution permission of the new process image file(including script file) is only checked on aarch64.
&nbsp;
- **ELOOP**
There is a loop in symbolic links encountered when resolving the path or file argument.
&nbsp;
- **ENAMETOOLONG**
A component of a pathname is longer than {NAME\_MAX}.
&nbsp;
- **ENOENT**
A component of path or file is not corresponding to an existing file or path or file is an empty string.
&nbsp;
- **ENOTDIR**
A component of the new process image file's path prefix names an existing file that is not corresponding to a directory or a symbolic link to a directory, or the new process image file's pathname contains at least one non-slash character and ends with one or more trailing slash characters and the last pathname component names an existing file that is not corresponding to a directory nor a symbolic link to a directory.
&nbsp;
The exec functions, except for execlp(), execvp() and execvpe(), fail if:

- **ENOEXEC**
The new process image file has the appropriate access permission but the format is unrecognized.
&nbsp;
The fexecve() function fails if:

- **EBADF**
The fd argument is not a valid file descriptor open for executing.
&nbsp;

The exec functions may fail if:

- **ENOMEM**
The memory requirement of new process image exceeds the limit allowed by the hardware or system-imposed memory management constraints.
&nbsp;
The exec functions, except for fexecve(), fail if:

- **ELOOP**
More than {SYMLOOP\_MAX} symbolic links were encountered when resolving of the path or file argument.
&nbsp;
- **ENAMETOOLONG**
The length of the path argument or the length of the pathname constructed from the file argument exceeds {PATH\_MAX}, or the intermediate result length of pathname resolving a symbolic link exceeds {PATH\_MAX}.
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
Currently, the logic of Hongmeng processing parameters is different from that of Linux. Linux processes argv and envp first, and then reads data in path. Hongmeng reads data in path first, and then process argv and envp. If path and argv in the input parameters are incorrect, the returned error codes are inconsistent.
