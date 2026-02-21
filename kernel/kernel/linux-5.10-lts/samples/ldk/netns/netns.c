// SPDX-License-Identifier: GPL-2.0
#include <linux/pid_namespace.h>
#include <net/net_namespace.h>
#include <trace/hooks/liblinux.h>

#define ROOT_NET_NSID 8189

static void hook_net_cookie_init(void *__data, struct net *net)
{
	u64 nsid = ROOT_NET_NSID;
	atomic64_set(&net->net_cookie, nsid);
}
INIT_VENDOR_HOOK(ldk_vh_net_cookie_init, hook_net_cookie_init);

static void hook_set_net_nsid(void *__data, struct net *net, struct net *old_net)
{
	u64 res = atomic64_read(&old_net->net_cookie);
	if (!net_eq(net, &init_net)) {
		res |= BATCH_FLAG;
	}
	atomic64_set(&net->net_cookie, res);
}
INIT_VENDOR_HOOK(ldk_vh_set_net_nsid, hook_set_net_nsid);

struct net * __override get_net_ns_by_fd(int fd)
{
	struct net *net = NULL;
	trace_ldk_vh_devhost_net_socket_get_ns_by_fd(fd, &net);
	if (net == NULL) {
		net = ERR_PTR(-EINVAL);
	}

	return net;
}

struct net * __override get_net_ns_by_pid(pid_t pid)
{
	struct net *net = NULL;
	trace_ldk_vh_devhost_net_socket_get_ns_by_pid((int)pid, &net);
	if (net == NULL) {
		net = ERR_PTR(-EINVAL);
	}

	return net;
}
