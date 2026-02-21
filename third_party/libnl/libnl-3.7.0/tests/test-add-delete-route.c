#include <netlink/cli/utils.h>
#include <netlink/cli/route.h>
#include <netlink/cli/link.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <params.h>


int main(int argc, char argv[])
{
    struct nl_sock *sk;
    struct rtnl_route *route;
    struct nl_cache *link_cache, *route_cache;
    char dst_addr[] = DST_ADDR;
    char nexthop[] = NEXTHOP;
    int err = 0;

    sk = nl_cli_alloc_socket();
    nl_cli_connect(sk, NETLINK_ROUTE);
    link_cache = nl_cli_link_alloc_cache(sk);
    route_cache = nl_cli_route_alloc_cache(sk, 0);
    route = nl_cli_route_alloc();

    nl_cli_route_parse_dst(route, dst_addr);
    nl_cli_route_parse_nexthop(route, nexthop, link_cache);

    if ((err = rtnl_route_add(sk, route, NLM_F_EXCL)) < 0) {
        printf("Unable to add route: %s", nl_geterror(err));
        goto END;
    }

    if ((err = rtnl_route_delete(sk, route, NLM_F_EXCL)) < 0) {
        printf("Unable to delete route: %s", nl_geterror(err));
        goto END;
    }

END:
    rtnl_route_put(route);
    nl_cache_put(link_cache);
    nl_cache_put(route_cache);
    nl_socket_free(sk);
    return err;
}
