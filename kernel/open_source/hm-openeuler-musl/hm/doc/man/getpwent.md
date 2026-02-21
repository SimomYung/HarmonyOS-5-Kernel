# getpwent()
Get a password entry from the /etc/passwd file

## Synopsis
```c
#include <grp.h>

struct passwd *getpwent(void);
```
## Arguments

N/A

## Library
libc

## Description
The getpwent() function gets a pointer to the structure containing the fields of the record in the password database. The first call will get the first entry from the /etc/passwd file, and each subsequent call will get the subsequent entries in turn.

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
The getpwent() function returns NULL if there are no more entries or an error occurred.  If an error occurs, errno is set appropriately.
&nbsp;

- **A pointer to the passwd structure**
The getpwent() function successfully gets a pointer to the structure containing the fields of the record in the password database.

## Errors

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
The following example uses getpwent() to get the subsequent entries in turn from the /etc/passwd file. The function retrieves the first entry after calling setpwent().
```c
	#include <sys/types.h>
	#include <grp.h>
	...
	struct passwd *pwd1 = NULL;
	struct passwd *pwd2 = NULL;

	setpwent();
	pwd1 = getpwent();
	if (pwd1 == NULL)
		...handle error...

	pwd2 = getpwent();
	if (pwd2 == NULL || pwd2 == pwd1)
		...handle error...

	setpwent();
	pwd2 = getpwent();
	if (pwd2 != pwd1)
		...handle error...
	endpwent();
```
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
