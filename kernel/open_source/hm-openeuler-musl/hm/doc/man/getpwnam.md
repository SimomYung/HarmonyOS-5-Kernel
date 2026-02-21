# getpwnam()
Get a password entry that matches the username

## Synopsis
```c
#include <sys/types.h>

#include <grp.h>

struct passwd *getpwnam(const char *name);
```
## Arguments

- **name**
The username.

## Library
libc

## Description
The getpwnam() function gets a pointer to the structure containing the fields of the record in the password database that matches the username *name*.

The passwd structure is defined in &lt;pwd.h&gt; as follows:
```c
struct passwd {
	char   *pw_name;	/* username */
	char   *pw_passwd;	/* user password */
	uid_t   pw_uid;		/* user ID */
	gid_t   pw_gid;		/* group ID */
	char   *pw_gecos;	/* user information */
	char   *pw_dir;		/* home directory */
	char   *pw_shell;	/* shell program */
};
```

## Returns

- **NULL**
The getpwnam() function returns NULL if the username does not match any entry. If an error occurs, function returns NULL and errno is set appropriately.
&nbsp;

- **A pointer to the passwd structure**
The getpwnam() function successfully gets a pointer to the structure containing the fields of the record in the password database that matches the username.

## Errors

- **0 or ENOENT or ESRCH or EBADF or EPERM or ...**
The given username was not found.
&nbsp;

- **EINTR**
A signal was caught.
&nbsp;

- **EIO**
I/O error.
&nbsp;

- **EMFILE**
The maximum allowable number of open file descriptors for the process has reached the limit.
&nbsp;

- **ENFILE**
The maximum allowable number of files has reached the limit in the system.
&nbsp;

- **ENOMEM**
Insufficient memory to allocate the passwd structure.
&nbsp;

- **ERANGE**
Insufficient buffer space supplied.

## Examples
The following example uses getpwnam() to obtains a pointer to the structure containing fields recorded in the password database that matches the username *root*.
```c
    #include <sys/types.h>
    #include <grp.h>
    ...
    struct passwd *pwd = NULL;
    char *username = "root";
    pwd = getpwnam(username);
    if (pwd == NULL)
        ...handle error...

```
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
