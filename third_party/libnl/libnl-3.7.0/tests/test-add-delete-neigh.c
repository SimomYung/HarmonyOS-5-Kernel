#include <netlink/cli/utils.h>
#include <netlink/cli/neigh.h>
#include <netlink/cli/link.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <params.h>


int main(int argc, char *argv[])
{
    struct nl_sock *sk;
    struct rtnl_neigh *neigh;
    struct nl_cache *link_cache;
    int err = 0, ok = 0, nlflags = NLM_F_REPLACE | NLM_F_CREATE;
    char lladdr[] = "AA:BB:CC:DD:EE:FF";

    sk = nl_cli_alloc_socket();
    nl_cli_connect(sk, NETLINK_ROUTE);
    link_cache = nl_cli_link_alloc_cache(sk);
    neigh = nl_cli_neigh_alloc();

    nl_cli_neigh_parse_dst(neigh, DST_ADDR);
    nl_cli_neigh_parse_lladdr(neigh, lladdr);
    nl_cli_neigh_parse_dev(neigh, link_cache, DEV_NAME);

    if ((err = rtnl_neigh_add(sk, neigh, nlflags)) < 0){
        printf("Unable to add neighbour: %s\n",nl_geterror(err));
        goto END;
    } 

    if ((err = rtnl_neigh_delete(sk, neigh, nlflags)) < 0){
        printf("Unable to add neighbour: %s\n",nl_geterror(err));
        goto END;
    }

END:   
    nl_socket_free(sk);
    nl_cache_put(link_cache);
    rtnl_neigh_put(neigh);
    return err;
}
