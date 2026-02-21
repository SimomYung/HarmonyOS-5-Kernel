#include <netlink/socket.h>
#include <netlink/netlink.h>
#include <netlink/cli/utils.h>
#include <netlink/cli/link.h>
#include <netlink/route/link.h>
#include <unistd.h>


static int self_def_cb = NL_CB_DEBUG;


int main(int argc, char *argv[])
{
    struct nl_sock *sk;
    struct nl_cache *link_cache;
    struct rtnl_link *link;
    struct nl_addr *addr;
    struct nl_cb *cb;
    int err = 0, ifindex, pid;
    char *buf;

    cb = nl_cb_alloc(self_def_cb);
    pid = getpid();

    if (!(sk = nl_socket_alloc_cb(cb))) {
        nl_cli_fatal(ENOBUFS, "Unable to allocate netlink socket\n");
    }
    nl_cli_connect(sk, NETLINK_ROUTE);
    nl_socket_disable_seq_check(sk);
    nl_socket_disable_auto_ack(sk);
    nl_socket_set_local_port(sk, pid);
    nl_join_groups(sk, pid);
    nl_socket_drop_membership(sk, pid);
    nl_socket_set_peer_port(sk, 0);

    link_cache = nl_cli_link_alloc_cache(sk);
    link = nl_cli_link_alloc();

    if (err = nl_socket_get_peer_port(sk)){
        printf("peer_port %d\n", err);
        goto END;
    }    
    if (err = nl_socket_use_seq(sk))
        printf("sk->s_seq_next %d\n", err);
    
    if ((ifindex = rtnl_link_get_ifindex(link)) == 0){
        printf("ifindex is not set, %d\n", ifindex);
        rtnl_link_set_ifindex(link, 1);
    };

    if (rtnl_link_get(link_cache, 1)){
        printf("now link is cached\n");
    }else{
        nl_cache_add(link_cache, (struct nl_object *)link);
    };

    if ((err = rtnl_link_add(sk, link, AF_INET)) < 0){
        printf("Unable to add link %s\n", nl_geterror(err));
        goto END;
    }

END:
    nl_cb_put(cb);
    nl_socket_free(sk);
    nl_cache_put(link_cache);
    rtnl_link_put(link);
    return err;
}
