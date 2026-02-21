# gethostbyaddr()

Convert network address to correspond host name

## Synopsis

```c
#include <netdb.h>
#include <sys/socket.h>       /* for AF_INET */
extern int h_errno;

struct hostent *gethostbyaddr(const void *addr,
                              socklen_t len, int type);
```

## Arguments

- **addr**
  The host address you want to query. Even though *addr* is a void *, you actually want to pass in a pointer to a struct in_addr.
  &nbsp;
- **len**
  Length of \**addr*, should be sizeof(struct in_addr).
  &nbsp;
- **type**
  Address type, valid address types are AF_INET and AF_INET6.

## Library

libc

## Description

The gethostbyaddr() function takes a struct in_addr or struct in6_addr and brings you up a corresponding host name (if there is one), so it's sort of the reverse of gethostbyname().

## Returns

The gethostbyaddr() function returns a pointer to *hostent* structure or a null pointer if an error occurs. On error, the h_errno variable holds an error number. When non-NULL, the return value may point at static data.
&nbsp;
The *hostent* structure is defined in &lt;netdb.h&gt;. As follows:

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
  An array of alternative names for the host, terminated by a null pointer.
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

## Errors

The variable *h_errno* can have the following values:

- **HOST_NOT_FOUND**
  The specified host is unknown.
  &nbsp;
- **NO_DATA**
  The requested name is valid but does not have an IP address. Another type of request to the name server for this domain may return an answer. The constant NO_ADDRESS is a synonym for NO_DATA.
  &nbsp;
- **NO_RECOVERY**
  A nonrecoverable name server error occurred.
  &nbsp;
- **TRY_AGAIN**
  A temporary error occurred on an authoritative name server. Try again later.

## Example

```C
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>

int main(int *argc, char **argv)
{
    struct sockaddr_in saddr = {0};
    socklen_t len = 4;
    struct hostent *hptr = NULL;

    inet_aton("127.0.0.1", &saddr.sin_addr);
    hptr = gethostbyaddr((void *)&saddr.sin_addr, len, AF_INET);
    if (hptr == NULL) {
        printf("test gethostbyaddr error: %s\n", strerror(errno));
        return -1;
    } else {
        printf("127.0.0.1 host name: %s\n", hptr->h_name);
        return 0;
    }

    return 0;
}
```

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
