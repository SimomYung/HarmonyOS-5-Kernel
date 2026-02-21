# getservbyname()

Retrieve service information corresponding to a service name and protocol

## Synopsis

```c
#include <netdb.h>

struct servent *getservbyname(const char *name, const char *proto);
```

## Arguments

- **name**
  Pointer to a service *name* string.
  &nbsp;
- **proto**
  A pointer to a null-terminated protocol name.

## Library

libc

## Description

The getservbyname() function returns a servent structure for the entry from the database that matches the service name *name* using protocol *proto*. If this pointer is NULL, any protocol will be matched. The servent structure contains information about the service:

```c
/* Description of data base entry for a single service.  */
struct servent {
    char  *s_name;       /* official service name */
    char **s_aliases;    /* alias list */
    int    s_port;       /* port number */
    char  *s_proto;      /* protocol to use */
}
```

## Returns

Returns a pointer to servent structure on success, or NULL on error.

## Errors

N/A

## Example

&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
