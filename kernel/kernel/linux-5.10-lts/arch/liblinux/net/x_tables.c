/* SPDX-License-Identifier: GPL-2.0-only */
#include <linux/netfilter/x_tables.h>
#include <linux/err.h>

static const char *const xt_prefix[NFPROTO_NUMPROTO] = {
	[NFPROTO_UNSPEC] = "x",
	[NFPROTO_IPV4]   = "ip",
	[NFPROTO_ARP]    = "arp",
	[NFPROTO_BRIDGE] = "eb",
	[NFPROTO_IPV6]   = "ip6",
};

struct xt_table * __override xt_request_find_table_lock(struct net *net, u_int8_t af,
							const char *name)
{
	struct xt_table *t = xt_find_table_lock(net, af, name);

#ifdef CONFIG_MODULES
	if (IS_ERR(t)) {
		/*
		 * for table not initialized, just return ENOENT
		 * override because end of this chain not impl, request_module always return ENOMEM
		 * request_module->__request_module->call_modprobe->call_usermodehelper_setup
		 */
		if (PTR_ERR(t) == -ENOENT) {
			return t;
		}

		int err = request_module("%stable_%s", xt_prefix[af], name);
		if (err < 0)
			return ERR_PTR(err);
		t = xt_find_table_lock(net, af, name);
	}
#endif

	return t;
}
