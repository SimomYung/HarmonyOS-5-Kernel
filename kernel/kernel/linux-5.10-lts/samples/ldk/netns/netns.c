// SPDX-License-Identifier: GPL-2.0
#include <linux/pid_namespace.h>
#include <net/net_namespace.h>
#include <trace/hooks/liblinux.h>

static void hook_set_net_nsid(void *__data, struct net *net, unsigned int nsid)
{
	net->nsid = net_eq(net, &init_net) ? nsid : (nsid | BATCH_FLAG);
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
