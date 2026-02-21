# shmctl()
System V shared memory operations

## Synopsis
```c
#include <sys/shm.h>

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

## Arguments

- **shmid**
Identify the shared memory.
&nbsp;
- **cmd**
Specify the control operations.
&nbsp;
- **buf**
Buffer used to store information of the message queue.

## Library
libc

## Description
The shmctl() performs the control operation specified by *cmd* on the System V shared memory segment whose identifier is given in *shmid*.

The *buf* argument is a pointer to a shmid_ds structure, defined in <sys/shm.h> as follows:
```c
	struct shmid_ds {
		struct ipc_perm shm_perm;    /* Ownership and permissions */
		size_t          shm_segsz;   /* Size of segment (bytes) */
		time_t          shm_atime;   /* Last attach time */
		time_t          shm_dtime;   /* Last detach time */
		time_t          shm_ctime;   /* Creation time/time of last
										modification via shmctl() */
		pid_t           shm_cpid;    /* PID of creator */
		pid_t           shm_lpid;    /* PID of last shmat(2)/shmdt(2) */
		shmatt_t        shm_nattch;  /* No. of current attaches */
		...
	};
```
The ipc_perm structure is defined as follows：
```c
	struct ipc_perm {
		key_t          __key;    /* Key supplied to shmget(2) */
		uid_t          uid;      /* Effective UID of owner */
		gid_t          gid;      /* Effective GID of owner */
		uid_t          cuid;     /* Effective UID of creator */
		gid_t          cgid;     /* Effective GID of creator */
		unsigned short mode;     /* Permissions + SHM_DEST and
									SHM_LOCKED flags */
		unsigned short __seq;    /* Sequence number */
	};
```

Valid values for cmd are:

* IPC_STAT
Copy information from the kernel data structure associated with *shmid* into the shmid_ds structure pointed to by *buf*. The caller must have read permission on the shared memory segment.
&nbsp;
* IPC_SET
Write the values of some members of the shmid_ds structure pointed to by *buf* to the kernel data structure associated with this shared memory segment, updating also its shm_ctime member.
&nbsp;
* IPC_RMID
Mark the segment to be destroyed. The segment will be destroyed after all processes detach it.
&nbsp;
* IPC_INFO
Return information about system-wide shared memory limits and parameters in the structure pointed to by *buf*. This structure is of type shminfo defined in <sys/shm.h> if the _GNU_SOURCE feature test macro is defined:
```c
	struct shminfo {
		unsigned long shmmax;
		unsigned long shmmin;
		unsigned long shmmni;
		unsigned long shmseg;
		unsigned long shmall;
	};
```
&nbsp;
* SHM_INFO
Return a shm_info structure whose fields contain information about system resources consumed by shared memory. This structure is defined in <sys/shm.h> if the _GNU_SOURCE feature test macro is defined:
```c
	  struct shm_info {
		  int           used_ids;
		  unsigned long shm_tot;
		  unsigned long shm_rss;
		  unsigned long shm_swp;
		  unsigned long swap_attempts;
		  unsigned long swap_successes;
	  };
```
&nbsp;
* SHM_STAT
Return a shmid_ds structure as for IPC_STAT.
&nbsp;
* SHM_STAT_ANY
Return a shmid_ds structure as for SHM_STAT.

## Returns

A successful IPC_INFO or SHM_INFO operation returns the index of the highest used entry in the kernel's internal array recording information about all shared memory segments. (This information can be used with repeated SHM_STAT or SHM_STAT_ANY operations to obtain information about all shared memory segments on the system.) A successful SHM_STAT operation returns the identifier of the shared memory segment whose index was given in *shmid*. Other operations return 0 on success.
&nbsp;
On error, -1 is returned, and *errno* is set to indicate the error.

## Errors

- **EACCES**
IPC_STAT or SHM_STAT is requested and shm_perm.mode does not allow read access for *shmid*.
&nbsp;
- **EFAULT**
The argument *cmd* has value IPC_SET or IPC_STAT but the address pointed to by *buf* isn't accessible.
&nbsp;
- **EINVAL**
*shmid* is not a valid identifier, or *cmd* is not a valid command.
&nbsp;
- **EPERM**
IPC_SET or IPC_RMID is attempted, and the effective user ID of the calling process is not that of the creator (found in shm_perm.cuid), or the owner (found in shm_perm.uid).
&nbsp;
- **EIDRM**
*shmid* points to a removed identifier.

## Example
&nbsp;

## Classification
POSIX.1-2001, POSIX.1-2008, SVr4.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
