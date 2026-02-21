# fpathconf(), pathconf()
Get configuration values for files

## Synopsis
```c
#include <unistd.h>

long fpathconf(int fd, int name);
long pathconf(const char *path, int name);
```

## Arguments

- **fd**
The open file descriptor.
&nbsp;
- **path**
The filename path.
&nbsp;
- **name**
A value for the configuration option.

## Library
libc

## Description

The fpathconf() function gets a value for the configuration option *name* for the open file descriptor *fd*. The pathconf() function gets a value for configuration option *name* for the filename *path*. The corresponding macros defined in &lt;unistd.h&gt; are minimum values; if an application wants to take advantage of values which may change, a call to fpathconf() or pathconf() can be made, which may yield more liberal results.
&nbsp;

Setting *name* equal to one of the following constants returns the following configuration options:
- **_PC_LINK_MAX**
	The maximum number of links to the file. If *fd* or *path* refer to a directory, then the value applies to the whole directory. The corresponding macro is _POSIX_LINK_MAX.
&nbsp;
- **_PC_MAX_CANON**
	The maximum length of a formatted input line, where *fd* or *path* must refer to a terminal. The corresponding macro is _POSIX_MAX_CANON.
&nbsp;
- **_PC_MAX_INPUT**
	The maximum length of an input line, where *fd* or *path* must refer to a terminal. The corresponding macro is _POSIX_MAX_INPUT.
&nbsp;
- **_PC_NAME_MAX**
	The maximum length of a filename in the directory *path* or *fd* that the process is allowed to create. The corresponding macro is _POSIX_NAME_MAX.
&nbsp;
- **_PC_PATH_MAX**
	The maximum length of a relative pathname when *path* or *fd* is the current working directory. The corresponding macro is _POSIX_PATH_MAX.
&nbsp;
- **_PC_PIPE_BUF**
	The maximum number of bytes that can be written atomically to a pipe of FIFO.  For fpathconf(), *fd* should refer to a pipe or FIFO.  For fpathconf(), *path* should refer to a FIFO or a directory; in the latter case, the returned value corresponds to FIFOs created in that directory.  The corresponding macro is _POSIX_PIPE_BUF.
&nbsp;
- **_PC_NO_TRUNC**
	This returns nonzero if accessing filenames longer than _POSIX_NAME_MAX generates an error.  The corresponding macro is _POSIX_NO_TRUNC.
&nbsp;
- **_PC_VDISABLE**
	This returns nonzero if special character processing can be disabled, where *fd* or *path* must refer to a terminal.

## Returns

- **-1**
On error, -1 is returned and errno is set to indicate the cause of the error.
&nbsp;
If *name* corresponds to a maximum or minimum limit, and that limit is indeterminate, -1 is returned and errno is not changed. (To distinguish an indeterminate limit from an error, set errno to zero before the call, and then check whether errno is nonzero when -1 is returned.)
&nbsp;
If *name* corresponds to an option, a positive value is returned if the option is supported, and -1 is returned if the option is not supported.
&nbsp;

- **the current value of the option or limit**
Otherwise, the current value of the option or limit is returned. This value will not be more restrictive than the corresponding value that was described to the application in &lt;unistd.h&gt; or &lt;limits.h&gt; when the application was compiled.

## Errors

- **ENOENT**
(pathconf()) A component of *path* does not exist, or *path* is an empty string.
&nbsp;
- **EACCES**
(pathconf()) Search permission is denied for one of the directories in the path prefix of path.
&nbsp;
- **EBADF**
(fpathconf()) *fd* is not a valid file descriptor.
&nbsp;
- **EINVAL**
*name* is invalid or The implementation does not support an association of name with the specified file.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
