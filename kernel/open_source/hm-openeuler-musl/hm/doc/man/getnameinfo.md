# getnameinfo()

Look up the host name and service name information for a given struct sockaddr

## Synopsis

```c
#include <sys/socket.h>
#include <netdb.h>

int getnameinfo(const struct sockaddr *sa, socklen_t salen,
                char *host, socklen_t hostlen,
                char *serv, socklen_t servlen, int flags);
```

## Arguments

- **sa**
  A pointer to a generic socket address structure (of type sockaddr_in or sockaddr_in6)
  &nbsp;
- **salen**
  Size of \**sa*.
  &nbsp;
- **host**
  A pointer to a caller-allocated buffer. After the call, the buffer will be filled with a null-terminated string indicates host name. The caller can specify that no host name is required by providing a NULL *host* or a zero *hostlen*.
  &nbsp;
- **hostlen**
  The *host* buffer size.
  &nbsp;
- **serv**
  A pointer to a caller-allocated buffer. After the call, the buffer will be filled with a null-terminated string indicates service name. The caller can specify that no service name is required by providing a NULL *serv* or a zero *servlen*.
  &nbsp;
- **servlen**
  The *serv* buffer size.
  &nbsp;
- **flags**
  Modifies the behavior of getnameinfo().

  - **NI_NAMEREQD**

    If set, then an error is returned if the *hostname* cannot be determined.
    &nbsp;

  - **NI_DGRAM**

    If set, then the service is datagram (UDP) based rather than stream (TCP) based. This is required for the few ports (512-514) that have different services for UDP and TCP.
    &nbsp;

  - **NI_NOFQDN**

    If set, return only the *hostname* part of the fully qualified domain name for local hosts.
    &nbsp;

  - **NI_NUMERICHOST**

    If set, then the numeric form of the *hostname* is returned. (When not set, this will still happen in case the node's name cannot be determined.)
    &nbsp;

  - **NI_NUMERICSERV**

    If set, then the numeric form of the service address is returned. (When not set, this will still happen in case the service's name cannot be determined.)

## Library

libc

## Description

This function is the opposite of getaddrinfo(), that is, this function takes an already loaded struct sockaddr and does a name and service name lookup on it. It replaces the old gethostbyaddr() and getservbyport() functions.
&nbsp;
You have to pass in a pointer to a struct sockaddr (which in actuality is probably a struct sockaddr_in or struct sockaddr_in6 that you've cast) in the *sa* parameter, and the length of that struct in the *salen*.
&nbsp;
The resultant host name and service name will be written to the area pointed to by the *host* and *serv* parameters. Of course, you have to specify the max lengths of these buffers in *hostlen* and *servlen*.
&nbsp;
Finally, there are several flags you can pass, but here a a couple good ones. NI_NOFQDN will cause the *host* to only contain the host name, not the whole domain name. NI_NAMEREQD will cause the function to fail if the name cannot be found with a DNS lookup (if you don't specify this flag and the name can't be found, getnameinfo() will put a string version of the IP address in *host* instead).

## Returns

Returns zero on success, or non-zero on error. The error code is listed below. If the return value is non-zero, it can be passed to gai_strerror() to get a human-readable string.

- **EAI_AGAIN**
  The name could not be resolved at this time. Try again later.
  &nbsp;
- **EAI_BADFLAGS**
  The flags argument has an invalid value.
  &nbsp;
- **EAI_FAIL**
  A nonrecoverable error occurred.
  &nbsp;
- **EAI_FAMILY**
  The address family was not recognized, or the address length was invalid for the specified family.
  &nbsp;
- **EAI_MEMORY**
  Out of memory.
  &nbsp;
- **EAI\_NONAME**
  The name does not resolve for the supplied arguments. NI_NAMEREQD is set and the host's name cannot be located, or neither hostname nor service name were requested.
  &nbsp;
- **EAI_OVERFLOW**
  The buffer pointed to by *host* or *serv* was too small.
  &nbsp;
- **EAI_SYSTEM**
  Other system error, check *errno* for details.

## Errors

N/A

## Example

```C
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int *argc, char **argv)
{
    struct sockaddr_in6 sa; /* could be IPv4 if you want */
    char host[1024];
    char service[20];

    /* pretend sa is full of good information about the host and port... */

    getnameinfo(&sa, sizeof sa, host, sizeof host, service, sizeof service, 0);

    /* e.g. "www.example.com" */
    printf("   host: %s\n", host);
    /* e.g. "http"// code for a client connecting to a server */
    printf("service: %s\n", service);
    /* namely a stream socket to www.example.com on port 80 (http) */
    /* either IPv4 or IPv6 */
    return 0;
}
```

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
