# innetgr()

Check if the triplet (hostname, username, domainname) is a member of a netgroup query by **netgroup**.

## Synopsis

```c
#include <nss/netgroup.h>

int innetgr(const char *netgroup, const char *host, const char *user, const char *domain);

```

## Arguments 

- **netgroup** (length < 256 bytes) : netgroup name to query
- **host** (length < 256 bytes) : hostname which is in the netgroup queryed by paramater **netgroup**
- **user** (length < 256 bytes) : username which is in the netgroup queryed by paramater **netgroup**
- **domain** (length < 256 bytes) : domain which is in the netgroup queryed by paramater **netgroup**

## Library

libc

## Description

The innetgr function is used to check whether the string triplet (host, user, and domain) in a parameter exists in the netgroup parameter. The netgroup database is obtained by parsing the converter file in the computer's /etc directory and querying the corresponding information source. The name service converter is a file called nsswitch.conf that controls how clients or applications obtain network information. Each line in the file identifies a particular type of network information, such as hosts, passwords, and groups, followed by one or more sources of that information.

## Returns

These functions return 1 on success and 0 for failure.

## Files

/etc/netgroup
/etc/nsswitch.conf

Ensure that the lengths of the network group name and host, user, and name triplets in each line in the netgroup file are less than 256 characters.

## Errors

N/A

## Example

```c
int ret = innetgr("powerusers", NULL, "torvalds", NULL);
```

## Classification
This function is not in POSIX.1-2001, but innetgr() is available on most UNIX systems.

## Function Safety
TODO
