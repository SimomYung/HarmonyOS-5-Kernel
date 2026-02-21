# getaddrinfo()

Return information based on a particular host name (such as its IP address) and a service(such as port number)

## Synopsis

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res);
```

## Arguments

- **node**
  The host name that you’re interested in, can be either a host name, like "www.example.com", or an IPv4 or IPv6 address(passed as a string). This parameter can also be NULL if you’re using the AI_PASSIVE flag.
  &nbsp;
- **service**
  It can be a port number (passed as a string, like “80”), or it can be a service name, like “http” or “tftp” or “smtp” or “pop”, etc. Well-known service names can be found in the IANA Port List or in your /etc/services file.
  &nbsp;
- **hints**
  It's a pointer to the addrinfo structure, which contains the following fields:

```c
    struct addrinfo {
        int              ai_flags;
        int              ai_family;
        int              ai_socktype;
        int              ai_protocol;
        socklen_t        ai_addrlen;
        struct sockaddr *ai_addr;
        char            *ai_canonname;
        struct addrinfo *ai_next;
    };
```

  - **ai_family** : This field specifies the desired address family for the returned addresses. You can set *ai_family* to AF_UNSPEC which tells getaddrinfo() to look for both IPv4 and IPv6 addresses. You can also restrict yourself to one or the other with AF_INET or AF_INET6.
  &nbsp;
  - **ai_socktype**: This field specifies the preferred socket type, for example SOCK_STREAM or SOCK_DGRAM. Specifying 0 in this field indicates that socket addresses of any type can be returned by getaddrinfo().
  &nbsp;
  - **ai_protocol**: This field specifies the protocol for the returned socket addresses. Specifying 0 in this field indicates that socket addresses with any protocol can be returned by getaddrinfo().
  &nbsp;
  - **ai_flags**: This field specifies additional options, described below. Multiple flags are specified by bitwise OR-ing them together.
  &nbsp;
- **res**
  The *res* will now point to a linked list of struct addrinfo, and you can go through this list to get all the addresses that match what you passed in with the hints.

## Library

libc

## Description

getaddrinfo() is a function that will return information on a particular host name (such as its IP address) and load up a struct sockaddr for you, taking care of the gritty details (like if it’s IPv4 or IPv6). It replaces the old functions gethostbyname() and getservbyname().

## Returns

Returns zero on success, or nonzero on error. If it returns nonzero, you can use the function gai_strerror() to get a printable version of the error code in the return value.

## Errors

- **EAI_ADDRFAMILY**
  The specified network host does not have any network addresses in the requested address family.
  &nbsp;

- **EAI_AGAIN**
  The name server returned a temporary failure indication. Try again later.
  &nbsp;

- **EAI_BADFLAGS**
  **hints.ai_flags** contains invalid flags; or, **hints.ai_flags** included AI_CANONNAME and name was NULL.
  &nbsp;

- **EAI_FAIL**
  The name server returned a permanent failure indication.
  &nbsp;

- **EAI_FAMILY**
  The requested address family is not supported.
  &nbsp;

- **EAI_MEMORY**
  Out of memory.
  &nbsp;

- **EAI_NODATA**
  The specified network host exists, but does not have any network addresses defined.
  &nbsp;

- **EAI\_NONAME**
  When hints' *ai_family* isn't compatible with node's family type, and no other related *ai_flags* set.
  &nbsp;

- **EAI_SERVICE**
  The requested service is not available for the requested socket type. It may be available through another socket type. For  example, this error could occur if service was "shell" (a service available only on stream sockets), and either *hints.ai_protocol* was IPPROTO_UDP, or *hints.ai_socktype* was SOCK_DGRAM; or the error could occur if service was not NULL,  and *hints.ai_socktype* was SOCK_RAW (a socket type that does not support the concept of services).
  &nbsp;

- **EAI_SOCKTYPE**
  The requested socket type is not supported. This could occur, for example, if *hints.ai_socktype* and *hints.ai_protocol* are inconsistent (e.g., SOCK_DGRAM and IPPROTO_TCP, respectively).
  &nbsp;

- **EAI_SYSTEM**
  Other system error, check errno for details.

## Example

```C
/* code for a client connecting to a server
 * namely a stream socket to www.example.com on port 80 (http)
 * either IPv4 or IPv6
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

int main(int *argc, char **argv)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /* use AF_INET6 to force IPv6 */
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("www.example.com", "http", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    /* loop through all the results and connect to the first we can */
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("connect");
            close(sockfd);
            continue;
        }

        break; /* if we get here, we must have connected successfully */
    }

    if (p == NULL) {
        /* looped off the end of the list with no connection */
        fprintf(stderr, "failed to connect\n");
        exit(2);
    }

    freeaddrinfo(servinfo); /* all done with this structure */

    return 0;
}
```

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
