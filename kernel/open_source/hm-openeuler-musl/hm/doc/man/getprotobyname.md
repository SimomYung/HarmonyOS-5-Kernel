# getprotobyname()

Retrieve protocol information corresponding to a protocol name

## Synopsis

```c
#include <netdb.h>

struct protoent *getprotobyname(const char *name);
```

## Arguments

- **name**
  Pointer to a protocol name string, "tcp", "udp", for example. Only protocol name in lowercase is supported now.

## Library

libc

## Description

The  getprotobyname() function returns a *protoent* structure for the entry from the database that matches the protocol name *name*. The  protoent structure contains information about the protocol:

```c
/* Description of data base entry for a single protocol.  */
struct protoent
{
  char *p_name;         /* Official protocol name.  */
  char **p_aliases;     /* Alias list.  */
  int p_proto;          /* Protocol number.  */
};
```

## Returns

Returns a pointer to protoent structure on success, or NULL on error. If the parameter name is in uppercase, no protocol matches and a NULL pointer will be returned.

## Errors
&nbsp;
## Example

```C
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int *argc, char **argv)
{
  struct protoent *servptr = NULL;
  int fd;

  servptr = getprotobyname("tcp");
  if (servptr != NULL) {
    fd = socket(AF_INET, SOCK_STREAM,servptr->p_proto);
  }

  return 0;
  ...
}
```

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
