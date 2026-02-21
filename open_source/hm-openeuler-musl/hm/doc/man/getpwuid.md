# getpwuid(), getpwuid\_r()
Get a password entry that matches the user ID

## Synopsis
```c
#include <sys/types.h>
#include <pwd.h>

struct passwd *getpwuid(uid_t uid);
int getpwuid_r(uid_t uid, struct passwd *pw, char *buf, size_t size, struct passwd **res);
```
## Arguments

- **uid**
The user ID.
&nbsp;
- **pw**
A pointer to a passwd structure where the function can store entry.
&nbsp;
- **buf**
A block of memory that the function can use to allocate storage referenced by passwd structure.
&nbsp;
- **size**
The size of block that *buf* points to, in characters.
&nbsp;
- **res**
The address of a pointer to a passwd structure. If getpwuid\_r() finds the entry, it stores a pointer to *pw* in location indicated by *res*; otherwise the function stores a NULL pointer there.

## Library
libc

## Description
The getpwuid() function gets a pointer to the structure containing the fields of the record in the password database that matches the user ID *uid*.
&nbsp;

The getpwuid\_r() function is a reentrant version of getpwuid(). It gets the fields of the record that matches the *uid*. The function updates passwd structure pointed to by *pw* and stores a pointer to that structure at location pointed by *res*. The structure contains an entry from user database with given *uid*. And it stores a NULL pointer at location pointed by *res* on error or if it can't find requested entry.

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
The getpwuid() function returns a pointer to the entry, or NULL if the matching entry is not found or an error occurs.

The getpwuid\_r() function returns 0, and sets the location pointed by *res* to *pw* on success. Otherwise, error number is returned, and NULL is stored in the location pointed by *res*.

- **NULL**
The getpwuid() function returns NULL if the id of user does not match any entry. If an error occurs, function returns NULL and errno is set appropriately.
&nbsp;

- **A pointer to the passwd structure**
The getpwuid() function successfully gets a pointer to the structure containing the fields of the record in the password database that matches the user ID.
&nbsp;

- **0**
The getpwuid\_r() function returns 0, and sets the location pointed by *res* to *pw* on success.
&nbsp;

- **Errors**
The getpwuid\_r() function returns an error number when it fails. And NULL is stored in the location pointed by *res*.


## Errors
- **0 or ENOENT or ESRCH or EBADF or EPERM or ...**
The given user id was not found.
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
Insufficient memory to allocate passwd structure.
&nbsp;

- **ERANGE**
Insufficient buffer space supplied.

## Examples
The following example uses getpwuid() to obtains a pointer to the structure containing fields recorded in the password database that matches the user ID *0*.
```c
	#include <sys/types.h>
	#include <grp.h>
	...
	struct passwd *pwd = NULL;
	uid_t uid = 0;
	pwd = getpwuid(uid);
	if (pwd == NULL)
		...handle error...

```
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
