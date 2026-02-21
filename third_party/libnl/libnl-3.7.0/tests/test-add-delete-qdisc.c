#include <netlink/cli/utils.h>
#include <netlink/cli/tc.h>
#include <netlink/cli/qdisc.h>
#include <netlink/cli/link.h>
#include <netlink-private/route/tc-api.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <params.h>


static int default_yes = 0, deleted = 0, interactive = 0;
static struct nl_sock *sk;

static void delete_cb(struct nl_object *obj, void *arg)
{
    struct rtnl_qdisc *qdisc = nl_object_priv(obj);
    struct nl_dump_params params = {
        .dp_type = NL_DUMP_LINE,
        .dp_fd = stdout,
    };
    int err;

        /* Ignore default qdiscs, unable to delete */
    if (rtnl_tc_get_handle((struct rtnl_tc *) qdisc) == 0)
         return;

    if (interactive && !nl_cli_confirm(obj, &params, default_yes))
         return;

    if ((err = rtnl_qdisc_delete(sk, qdisc)) < 0) {
         nl_cli_fatal(err, "Unable to delete qdisc: %s\n", nl_geterror(err));
    }
    deleted++;
}

static int test_delete_qdisc()
{
    struct rtnl_qdisc *qdisc;
    struct rtnl_tc *tc;
    struct nl_cache *link_cache, *qdisc_cache;
    struct nl_cli_tc_module *tm;
    struct rtnl_tc_ops *ops;
    char kind[] = "htb";
    int err = 0;

    sk = nl_cli_alloc_socket();
    nl_cli_connect(sk, NETLINK_ROUTE);
    link_cache = nl_cli_link_alloc_cache(sk);
    qdisc_cache = nl_cli_qdisc_alloc_cache(sk);
    qdisc = nl_cli_qdisc_alloc();
    tc = (struct rtnl_tc *) qdisc;
    nl_cli_tc_parse_dev(tc, link_cache, DEV_NAME);
    nl_cli_tc_parse_parent(tc, "root");

    if (!rtnl_tc_get_ifindex(tc)) {
        printf("You must specify a network device (--dev=XXX)");
        goto END;
    }

    if (!rtnl_tc_get_parent(tc)) {
        printf("You must specify a parent");
        goto END;
    }

    rtnl_tc_set_kind(tc, kind);
    if (!(ops = rtnl_tc_get_ops(tc))) {
        printf("Unknown qdisc \"%s\"", kind);
        goto END;
    }

    if (!(tm = nl_cli_tc_lookup(ops))) {
        nl_cli_fatal(ENOTSUP, "Qdisc type \"%s\" not supported.", kind);
        goto END;
    }


    nl_cache_foreach_filter(qdisc_cache, OBJ_CAST(qdisc), delete_cb, NULL);

END:
    nl_cache_put(link_cache);
    nl_cache_put(qdisc_cache);
    rtnl_qdisc_put(qdisc);
    nl_socket_free(sk);
    return err;
}

static int test_add_qdisc()
{
    struct rtnl_qdisc *qdisc;
    struct rtnl_tc *tc;
    struct nl_cache *link_cache;
    struct nl_cli_tc_module *tm;
    struct rtnl_tc_ops *ops;
    char kind[] = "htb";
    int err = 0, flags = NLM_F_CREATE | NLM_F_EXCL;

    if (!(sk = nl_socket_alloc())){
        printf("Unable to allocate netlink socket\n");
        return -1;
    }
    if ((err = nl_connect(sk, NETLINK_ROUTE)) < 0) {
        printf("Unable to connect netlink socket: %s\n", nl_geterror(err));
        nl_socket_free(sk);
        return -1;
    }
    link_cache = nl_cli_link_alloc_cache(sk);
    qdisc = nl_cli_qdisc_alloc();
    tc = (struct rtnl_tc *) qdisc;

    nl_cli_tc_parse_dev(tc, link_cache, DEV_NAME);
    nl_cli_tc_parse_parent(tc, "root");

    if (!rtnl_tc_get_ifindex(tc)){
        printf("You must specify a network device (--dev=XXX)\n");
        goto END;
    }

    if (!rtnl_tc_get_parent(tc)){
        printf("You must specify a parent\n");
        goto END;
    }

    rtnl_tc_set_kind(tc, kind);
    if (!(ops = rtnl_tc_get_ops(tc))){
        printf("Unknown qdisc \"%s\"\n", kind);
        goto END;
    }
    if (!(tm = nl_cli_tc_lookup(ops))){
        nl_cli_fatal(ENOTSUP, "Qdisc type \"%s\" not supported.\n", kind);
        goto END;
    }

    if ((err = rtnl_qdisc_add(sk, qdisc, flags)) < 0){
        printf("Unable to add qdisc: %s\n", nl_geterror(err));
        goto END;
    }

END:
    nl_cache_mngt_unprovide(link_cache);
    nl_cache_put(link_cache);
    rtnl_qdisc_put(qdisc);
    nl_socket_free(sk);
    return err;
}

int main(int args, char *argv[])
{
    int err = 0;
    if ((err = test_add_qdisc()) < 0) {
        printf("Unable to add qdisc:%s", nl_geterror(err));
    }
    if ((err = test_delete_qdisc()) < 0) {
        printf("Unable to delete qdisc:%s", nl_geterror(err));
    }
    return err;
}
