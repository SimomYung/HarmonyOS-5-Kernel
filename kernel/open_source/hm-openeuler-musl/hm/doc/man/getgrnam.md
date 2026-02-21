# getgrnam(), getgrnam\_r()
Get information about group with a given name

## Synopsis
```c
#include <grp.h>

struct group *getgrnam(const char* name);

int getgrnam_r(const char* name, struct group* gr, char *buf, size_t size, struct group **res);
```
## Arguments
- **name**
Name of group you want to get information about.
&nbsp;
- **gr**
A pointer to a group structure where function can store information about group.
&nbsp;
- **buf**
A buffer from which to allocate any memory required.
&nbsp;
- **size**
Size of *buf*.
&nbsp;
- **res**
Address of a pointer that getgrgid\_r() sets to same pointer as *gr* on success, or to NULL if function can't find group.

## Library
libc

## Description
The getgrnam() function lets a process gain more knowledge about group named *name*. This function uses a static buffer that's overwritten by each call.
&nbsp;
The getgrnam\_r() function obtains same information as getgrnam(), but stores retrieved group structure in space pointed to by *gr*. The string fields pointed to by members of group structure are stored in buffer *buf* of *size* buflen. A pointer to result (in case of success) or NULL (in case no entry was found or an error occurred) is stored in *res*.

## Returns
- **getgrnam()**
A pointer to an object of type struct group containing an entry from group database with a matching name, or NULL on error or failure to find an entry with a matching name.
- **getgrnam_r()**
Zero for success, or an error number if an error occurred.
## Errors
- **0 or ENOENT or ESRCH or EBADF or EPERM or ...**
The given name or gid was not found.
&nbsp;
- **EINTR**
A signal was caught.
&nbsp;
- **EIO**
I/O error.
&nbsp;
- **EMFILE**
Per-process limit on number of open file descriptors has been reached.
&nbsp;
- **ENFILE**
The system-wide limit on total number of open files has been reached.
&nbsp;
- **ENOMEM**
Insufficient memory to allocate group structure.
&nbsp;
- **ERANGE**
Insufficient storage was supplied via *buf* and *size* to contain group structure.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
