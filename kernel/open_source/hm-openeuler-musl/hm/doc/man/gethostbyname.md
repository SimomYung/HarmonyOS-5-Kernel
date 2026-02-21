# gethostbyname()

Determine the IP address of your local machine

## Synopsis

```c
#include <netdb.h>
extern int h_errno;

struct hostent *gethostbyname(const char *name);
```

## Arguments

- **name**
  The host *name* string, such as "www.huawei.com". The parameter name is case sensitive, for example localhost and LocalHost are different.

## Library

libc

## Description

The gethostbyname() function takes a string like "www.huawei.com", and returns a struct hostent which contains tons of information, including the IP address. (Other information is the official host name, a list of aliases, the address type, the length of the addresses, and the list of addresses.)

## Returns

The gethostbyname() function returns a pointer to hostent structure or a null pointer if an error occurs. On error, the *h_errno* variable holds an error number. It can be printed using the functions herror() or hstrerror(). These work just like the classic *errno*, perror(), and strerror(). When non-NULL, the return value may point at static data.
&nbsp;

The hostent structure is defined in &lt;netdb.h&gt;. As follows:

```c
struct hostent {
    char  *h_name;            /* official name of host */
    char **h_aliases;         /* alias list */
    int    h_addrtype;        /* host address type */
    int    h_length;          /* length of address */
    char **h_addr_list;       /* list of addresses */
}
#define h_addr h_addr_list[0] /* for backward compatibility */
```

The members of the hostent structure are:

- **h_name**
  The official name of the host.
  &nbsp;
- **h_aliase**
  An array of alternative names for the host, terminated by a null pointer. The first member of h\_aliase stores the same name with h\_name in the returned hostent struct.
  &nbsp;
- **h_addrtype**
  The type of address; always AF_INET or AF_INET6 at present.
  &nbsp;
- **h_length**
  The length of the address in bytes.
  &nbsp;
- **h_addr_list**
  An array of pointers to network addresses for the host (in network byte order), terminated by a null pointer.
  &nbsp;
- **h_addr**
  The first address in *h_addr_list* for backward compatibility.
  &nbsp;

We should note that the first member of *h_aliases* stores the same name with *h_name* in the returned hostent struct.

## Errors

The variable *h_errno* can have the following values:

- **HOST_NOT_FOUND**
  The specified host is unknown.
  &nbsp;
- **NO_RECOVERY**
  A nonrecoverable name server error occurred.
  &nbsp;
- **TRY_AGAIN**
  A temporary error occurred on an authoritative name server. Try again later.

## Example

```C
/* THIS IS A DEPRECATED METHOD OF GETTING HOST NAMES */
/* use getaddrinfo() instead! */

#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int i;
    struct hostent *he;
    struct in_addr **addr_list;

    if (argc != 2) {
        fprintf(stderr,"usage: ghbn hostname\n");
        return 1;
    }

    if ((he = gethostbyname(argv[1])) == NULL) {  /* get the host info */
        herror("gethostbyname");
        return 2;
    }

    /* print information about this host: */
    printf("Official name is: %s\n", he->h_name);
    printf("    IP addresses: ");
    addr_list = (struct in_addr **)he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) {
        printf("%s ", inet_ntoa(*addr_list[i]));
    }
    printf("\n");

    return 0;
}
```

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
