# getifaddrs()

The getifaddrs() gets local network interface information and store it in a linked list

## Synopsis

```c
#include <sys/types.h>
#include <ifaddrs.h>

int getifaddrs(struct ifaddrs **ifap);
```

## Arguments

- **ifap**
  It's a pointer points to a pinter to a listed list of struct ifaddrs describing the network interfaces of the local system.

```c
    struct ifaddrs {
        struct ifaddrs  *ifa_next;    /* Next item in list */
        char            *ifa_name;    /* Name of interface */
        unsigned int     ifa_flags;   /* Flags from SIOCGIFFLAGS */
        struct sockaddr *ifa_addr;    /* Address of interface */
        struct sockaddr *ifa_netmask; /* Netmask of interface */
        union {
            struct sockaddr *ifu_broadaddr;
            /* Broadcast address of interface */
            struct sockaddr *ifu_dstaddr;
            /* Point-to-point destination address */
        } ifa_ifu;
    #define              ifa_broadaddr ifa_ifu.ifu_broadaddr
    #define              ifa_dstaddr   ifa_ifu.ifu_dstaddr
        void            *ifa_data;    /* Address-specific data */
    };
```

## Library

libc

## Description

The getifaddrs() returns one or more ifaddrs structures, each of which corresponds with a network interface, and contains a particular IP, subnet mask, flags, destination IP and address-specific data.

## Returns

Returns zero on success, or -1 on error. If it returns nonzero, *errno* is set appropriately.

## Errors

getifaddrs() may fail and set *errno* for any of the errors specified for socket(), bind(), getsockname(), recvmsg(), sendto(), malloc(), or realloc().

## Example

```C
#include <sys/types.h>
#include <ifaddrs.h>
#include <stdio.h>

int main(int *argc, char **argv)
{
	struct ifaddrs *ifaddr, *ifa;
	int family, s, n;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return -1;
	}

	/* Walk through linked list, maintaining head pointer so we
		can free list later */

	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		/* Display interface name and family (including symbolic
			form of the latter for the common families) */

		printf("%-8s %s (%d)\n",
				ifa->ifa_name,
				(family == AF_PACKET) ? "AF_PACKET" :
				(family == AF_INET) ? "AF_INET" :
				(family == AF_INET6) ? "AF_INET6" : "???",
				family);

		/* For an AF_INET* interface address, display the address */

		if (family == AF_INET || family == AF_INET6) {
			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ? sizeof(struct sockaddr_in) :
					sizeof(struct sockaddr_in6),
					host, NI_MAXHOST,
					NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				exit(EXIT_FAILURE);
			}

			printf("\t\taddress: <%s>\n", host);

		}

		if (ifa->ifa_data != NULL) {
			struct if_stats *stats = ifa->ifa_data;

			printf("\t\tpackets_transmitted = %10u; "
				"packets_received = %10u\n"
					"\t\ttotal_bytes_transmitted = %10u; "
					"total_bytes_received   = %10u\n",
					stats->packets_transmitted,
					stats->packets_received,
					stats->total_bytes_transmitted,
					stats->total_bytes_received);
		}
	}

	freeifaddrs(ifaddr);
	return 0;
}
```

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
