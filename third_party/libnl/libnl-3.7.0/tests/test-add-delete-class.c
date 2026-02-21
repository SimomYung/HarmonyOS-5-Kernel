#include <netlink/cli/utils.h>
#include <netlink/cli/tc.h>
#include <netlink/cli/class.h>
#include <netlink/cli/link.h>
#include <netlink-private/route/tc-api.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <params.h>

static int default_yes = 0, deleted = 0, interactive = 0;
static struct nl_sock *sk;

static int test_add_class()
{
    struct rtnl_class *class;
    struct rtnl_tc *tc;
    struct nl_cache *link_cache;
    struct nl_cli_tc_module *tm;
    struct rtnl_tc_ops *ops;
    int err = 0, flags = NLM_F_CREATE | NLM_F_EXCL;
    char kind[] = "htb";
    char *rate[] = {DEV_NAME, "root", "htb", "--rate=100mbit"};

    sk = nl_cli_alloc_socket();
    nl_cli_connect(sk, NETLINK_ROUTE);
    link_cache = nl_cli_link_alloc_cache(sk);
    class = nl_cli_class_alloc();
    tc = (struct rtnl_tc *) class;

    nl_cli_tc_parse_dev(tc, link_cache, DEV_NAME);
    nl_cli_tc_parse_parent(tc, "root");
    if (!rtnl_tc_get_ifindex(tc)) {
        printf("You must specify a network device (--dev=XXX)\n");
        err = -1;
        goto END;
    }
    if (!rtnl_tc_get_parent(tc)) {
        printf("You must specify a parent (--parent=XXX)\n");
        err = -1;
        goto END;
    }

    rtnl_tc_set_kind(tc, kind);
    if (!(ops = rtnl_tc_get_ops(tc))) {
        printf("Unknown class \"%s\"\n", kind);
        err = -1;
        goto END;
    }
    if (!(tm = nl_cli_tc_lookup(ops))) {
        printf("class type \"%s\" not supported.\n", kind);
        err = -1;
        goto END;
    }
    tm->tm_parse_argv(tc, 4, rate);

    if ((err = rtnl_class_add(sk, class, flags)) < 0) {
        printf("Unable to add class: %s\n", nl_geterror(err));
        goto END;
    }

END:
    nl_cache_mngt_unprovide(link_cache);
    nl_cache_put(link_cache);
    rtnl_class_put(class);
    nl_socket_free(sk);
    return err;
}


static void delete_cb(struct nl_object *obj, void *arg)
{
    struct rtnl_class *class = nl_object_priv(obj);
    struct nl_dump_params params = {
        .dp_type = NL_DUMP_LINE,
        .dp_fd = stdout,
	};
    int err;

    if (interactive && !nl_cli_confirm(obj, &params, default_yes))
        return;

    if ((err = rtnl_class_delete(sk, class)) < 0)
        nl_cli_fatal(err, "Unable to delete class: %s\n", nl_geterror(err));

    deleted++;
}

static int test_delete_class()
{
    struct rtnl_class *class;
    struct rtnl_tc *tc;
    struct nl_cache *link_cache, *class_cache;
    struct nl_cli_tc_module *tm;
    struct rtnl_tc_ops *ops;
    char kind[] = "htb";
    int err = 0;
    
    sk = nl_cli_alloc_socket();
    nl_cli_connect(sk, NETLINK_ROUTE);
    link_cache = nl_cli_link_alloc_cache(sk);
    class = nl_cli_class_alloc();
    tc = (struct rtnl_tc *) class;

    nl_cli_tc_parse_dev(tc, link_cache, DEV_NAME);
    nl_cli_tc_parse_parent(tc, "root");
    if (!rtnl_tc_get_ifindex(tc)) {
        printf("You must specify a network device (--dev=XXX)\n");
        err = -1;
        goto END;
    }
    if (!rtnl_tc_get_parent(tc)) {
        printf("You must specify a parent (--parent=XXX)\n");
        err = -1;
        goto END;
    }
    rtnl_tc_set_kind(tc, kind);
    if (!(ops = rtnl_tc_get_ops(tc))) {
        printf("Unknown class \"%s\"\n", kind);
        err = -1;
        goto END;
    }
    class_cache = nl_cli_class_alloc_cache(sk, rtnl_tc_get_ifindex(tc));
    nl_cache_foreach_filter(class_cache, OBJ_CAST(class), delete_cb, NULL);

END:
    nl_cache_put(link_cache);
    nl_socket_free(sk);
    rtnl_class_put(class);
    return err;
}

int main(int argc, char *argv[])
{
    int err = 0;
    if ((err = test_add_class()) < 0) {
        printf("Unable to add class\n");
    }
    if ((err = test_delete_class()) < 0) {
        printf("Unable to delete class");
    }
    return err;
}
