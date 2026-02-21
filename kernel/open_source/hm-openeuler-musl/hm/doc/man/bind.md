# bind()
Bind a name to a socket

## Synopsis
```c
#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr,
	 socklen_t addrlen);
```

## Arguments

- **sockfd**
*sockfd* is a socket that has been created with socket().
&nbsp;
- **addr**
*addr* is a pointer to a sockaddr structure.
&nbsp;
- **addrlen**
*addrlen* specifies the size.

## Library
libc

## Description
This operation is called “assigning a name to a socket”.

## Returns

- **0**
On success, zero is returned.
&nbsp;

- **-1**
On error, -1 is returned, and errno is set appropriately.

## Errors

- **EAFNOSUPPORT**
Parameter error, the family is not supported or the network function is disabled.
&nbsp;
- **EADDRINUSE**
The given address is already in use.
&nbsp;
- **EBADF**
sockfd is not a valid descriptor.
&nbsp;
- **EINVAL**
The socket is already bound to an address.
&nbsp;
- **ENOTSOCK**
The file descriptor sockfd does not refer to a socket.
&nbsp;
- **ENOMEM**
Insufficient kernel memory was available.

## Examples
An example of the use of bind() with Internet domain sockets can be found in getaddrinfo().
The following example shows how to bind a stream socket in the UNIX (AF_UNIX) domain, and accept connections:
```c
       #include <sys/socket.h>
       #include <sys/un.h>
       #include <stdlib.h>
       #include <stdio.h>
       #include <string.h>

       #define MY_SOCK_PATH "/somepath"
       #define LISTEN_BACKLOG 50

       #define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

       int
       main(int argc, char *argv[])
       {
           int sfd, cfd;
           struct sockaddr_un my_addr, peer_addr;
           socklen_t peer_addr_size;

           sfd = socket(AF_UNIX, SOCK_STREAM, 0);
           if (sfd == -1)
               handle_error("socket");

           memset(&my_addr, 0, sizeof(struct sockaddr_un));
                               /* Clear structure */
           my_addr.sun_family = AF_UNIX;
           strncpy(my_addr.sun_path, MY_SOCK_PATH,
                   sizeof(my_addr.sun_path) - 1);

           if (bind(sfd, (struct sockaddr *) &my_addr,
                   sizeof(struct sockaddr_un)) == -1)
               handle_error("bind");

           if (listen(sfd, LISTEN_BACKLOG) == -1)
               handle_error("listen");

           /* Now we can accept incoming connections one
              at a time using accept(2) */

           peer_addr_size = sizeof(struct sockaddr_un);
           cfd = accept(sfd, (struct sockaddr *) &peer_addr,
                        &peer_addr_size);
           if (cfd == -1)
               handle_error("accept");

           /* Code to deal with incoming connection(s)... */

           /* When no longer required, the socket pathname, MY_SOCK_PATH
              should be deleted using unlink(2) or remove(3) */
       }
```

## Classification
POSIX 1003.1-2017

## Function Safety
N/A
