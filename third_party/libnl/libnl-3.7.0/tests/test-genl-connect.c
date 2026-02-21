#include <linux/genetlink.h>
#include <netlink/socket.h>
#include <netlink/cli/utils.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    struct nl_sock *sk;
    struct nl_cache *family_cache;
    struct nl_dump_params params = {
        .dp_type = NL_DUMP_LINE,
        .dp_fd = stdout,
	};
    int err = 0;

    sk = nl_socket_alloc();
    if ((err = genl_connect(sk)) < 0) {
        printf("Unable create socket: %s\n", nl_geterror(err));
        goto END;
    }    
    nl_socket_enable_auto_ack(sk);

    if (nl_socket_get_fd(sk) < 0) {
        printf("vaild socket\n");
	err = -1;
        goto END;
    }
    nl_socket_set_buffer_size(sk, 32655, 32655);
    family_cache = nl_cli_alloc_cache(sk, "generic netlink family", genl_ctrl_alloc_cache);
    nl_cache_dump(family_cache, &params);

END:
    nl_socket_free(sk);
    nl_cache_put(family_cache);
    return err;
}
