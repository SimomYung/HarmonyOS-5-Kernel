# open(), open64(), openat(), openat64()
Open a regular file or a directory or a device file

## Synopsis
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *filename, int flags);
int open(const char *filename, int flags, mode_t mode);
int open64(const char *filename, int flags);
int open64(const char *filename, int flags, mode_t mode);

int openat(int fd, const char *filename, int flags);
int openat(int fd, const char *filename, int flags, mode_t mode);
int openat64(int fd, const char *filename, int flags);
int openat64(int fd, const char *filename, int flags, mode_t mode);
```

## Arguments
- **filename**
The name of the file to open.
&nbsp;
- **flags**
File status or operating flags of open.
&nbsp;
- **mode**
The argument *mode* is used in case a new file is created, see the description below about *mode*.
&nbsp;
- **fd**
The file descriptor of a directory.
&nbsp;


## Library
libc

## Description
open() returns a file descriptor for the given *filename*. The returned file description records the file offset and the file status flags.
The argument *flags* must include one of the following access modes: O\_RDONLY, O\_WRONLY, or O\_RDWR. These request opening the file read-only, write-only, or read/write, respectively.
In addition, only O\_RDONLY, O\_WRONLY, O\_RDWR, O\_APPEND, O\_CREAT, O\_DIRECTORY, O\_EXCL, O\_TRUNC, O\_CLOEXEC, O\_NOFOLLOW, O\_NONBLOCK are supported now.
- O\_RDONLY
The file is opened for reading only.
- O\_WRONLY
The file is opened for writing only.
- O\_RDWR
The file is opened for reading and writing.
- O\_APPEND
The file is opened in append mode, and the file offset is positioned at the end of the file.
- O\_CREAT
If the file does not exist, it will be created.
- O\_DIRECTORY
If *filename* is not a directory, the open operation will fail.
- O\_EXCL
This flag is used with the O\_CREAT flag, if the file does not exist, this call creates the file. If the file exists, this call will fail.
- O\_TRUNC
If the file already exists and is a regular file and the access mode allows writing it will  be  truncatedto length 0. If the file is a FIFO or terminal device file, the O_TRUNC flag is ignored.
- O\_CLOEXEC
When exec() is called successfully, the file will be closed automatically.
- O\_NOFOLLOW
If *pathname* is a symbolic link, then the open fails.
- O\_NONBLOCK
The file is opened in nonblocking mode if possible.
- O\_SYNC
By the time write() returns, the data and metadata have been transferred to the
underlying handware. Just like each write() was followed by a call to fsync().
- O\_DSYNC
By the time write() returns, the data have been transferred to the underlying
handware.
- O\_PATH
  - Get a fd that indicate a file in VFS level.

  - The fd can be used by close(), fchdir(), fstat(), fstatfs(), dup(), fcntl() F\_DUPFD, fcntl() F\_GETFD F\_SETFD, fcntl() F_GETFL, and as the dirfd for other *at system calls.

  - When O_PATH is specified in flags, flag bits other than O\_CLOEXEC, O\_DIRCTORY and O\_NOFOLLOW are ignored.

  - O\_PATH | O\_NOFOLLOW can open a symlink not return ELOOP.
- O\_LARGEFILE
Any open call will added O\_LARGEFILE by libc(musl) silently.
- O\_DIRECT
File read/write is done directly to/from user-space buffers with minimize cache effects.
- O\_TMPFILE
Create an unnamed regular file. The *pathname* specifies a directory. The file will be unlinked when the last file descriptor is closed and the data will lose.

O_TMPFILE must be specified with one of O_RDWR or O_WRONLY.

Only tmpfs/devfs/ext2 support this flag.
&nbsp;

The argument *mode* is used in case a new file is created. This argument must be set when O\_CREAT or O\_TMPFILE is specified in *flags*.
&nbsp;
The following constants are defined for *mode*:
- S\_IRWXU  00700 user (file owner) has read, write, and execute permission
- S\_IRUSR  00400 user has read permission
- S\_IWUSR  00200 user has write permission
- S\_IXUSR  00100 user has execute permission
- S\_IRWXG  00070 group has read, write, and execute permission
- S\_IRGRP  00040 group has read permission
- S\_IWGRP  00020 group has write permission
- S\_IXGRP  00010 group has execute permission
- S\_IRWXO  00007 others have read, write, and execute permission
- S\_IROTH  00004 others have read permission
- S\_IWOTH  00002 others have write permission
- S\_IXOTH  00001 others have execute permission
&nbsp;

The following bits are also used in *mode*. Setting other bits in *mode* is unspecified.
- S\_ISUID  0004000 set-user-ID bit
- S\_ISGID  0002000 set-group-ID bit (see the usage of stat())
- S\_ISVTX  0001000 sticky bit (see the usage of stat())

Regards to 64 version, they follow:
```c
#define open64 open
#define openat64 openat
```
## Returns
On success, open() or openat() returns the new file descriptor. On error, -1 is returned and errno is set to indicate the error.

## Errors

- **EFAULT**
The address of the argument *filename* is invalid.
&nbsp;
- **ENOENT**
The O_CREAT is not set and named file dose not exist.
&nbsp;
- **ENOENT**
A directory component in *pathname* does not exist or is a dangling symbolic link.
&nbsp;
- **ENAMETOOLONG**
The argument *filename* is too long.
&nbsp;
- **EINVAL**
The argument *flags* does not include one of the following access modes: O\_RDONLY, O\_WRONLY, or O\_RDWR. If O\_TMPFILE flag is included, but O\_CREAT flag is not include.If O\_TMPFILE flag is included, but read flag (O\_RDWR or O\_WRONLY) is not included.
&nbsp;
- **EBADF**
The argument *fd* is not a valid file descriptor.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EROFS**
The argument *filename* refers to a file on a read-only filesystem and write access was requested.
&nbsp;
- **ENOTDIR**
The argument *filename* is a relative pathname and *fd* is a file descriptor referring to a file other than a directory. If O\_DIRECTORY flag was specified and *filename* was not a directory.
&nbsp;
- **EISDIR**
If O\_DIRECTORY flag was not specified and *filename* was a directory.
&nbsp;
- **ELOOP**
Too many symbolic links were encountered in resolving *filename*. If *filename* is a symbolic link, O\_NOFOLLOW is wrongly set.
&nbsp;
- **ENODEV**
The argument *filename* refers to a device special file and no corresponding device exists.
&nbsp;
- **EMFILE**
The per-process limit on the number of open file descriptors has been reached.
&nbsp;
- **EACCES**
The requested access to the file is not allowed.
&nbsp;
- **EPERM**
The operation is not permitted.
&nbsp;
- **EEXIST**
The *filename* already existed and O_CREAT and O_EXCL were used.
&nbsp;
- **ENOSPC**
*filename* was to be created but the device has no room for the new file.
&nbsp;
- **ENOSPC**
*pathname* refers to an executable image which is currently being executed and write access was requested.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
