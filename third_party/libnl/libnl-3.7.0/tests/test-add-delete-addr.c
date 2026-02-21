#include <netlink/cli/utils.h>
#include <netlink/cli/addr.h>
#include <netlink/cli/link.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <params.h>


int main(int argc, char *argv[])
{
    struct nl_sock *sock;
    struct rtnl_addr *addr;
    struct nl_cache *link_cache;
    int err = 0, nlflags = NLM_F_CREATE;

    sock = nl_cli_alloc_socket();
    nl_cli_connect(sock, NETLINK_ROUTE);
    link_cache = nl_cli_link_alloc_cache(sock);
    addr = nl_cli_addr_alloc();

    nl_cli_addr_parse_local(addr, IP);
    nl_cli_addr_parse_dev(addr, link_cache, DEV_NAME);
    
    if ((err = rtnl_addr_add(sock, addr, nlflags)) < 0) {
        printf("Unable to add route: %s", nl_geterror(err));
        goto END;
    }

    if ((err = rtnl_addr_delete(sock, addr, nlflags)) < 0) {
        printf("Unable to add route: %s", nl_geterror(err));
        goto END;
    }

END:
    rtnl_addr_put(addr);
    nl_cache_put(link_cache);
    nl_socket_free(sock);
    return err;
}
