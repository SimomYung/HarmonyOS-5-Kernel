/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: devhost net namespace operation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 29 21:16:45 CST 2023
 */

#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/version.h>
#include <net/sock.h>

#ifdef CONFIG_VENDOR_HOOKS
#include <trace/hooks/liblinux.h>
#endif

#include "net_socket_api.h"
#include "net_namespace.h"

static struct net *netns_arr[MAX_NAMESPACE_NR];
static DEFINE_MUTEX(ldk_namespace_lock);

static void lock_ldk_namespace(void)
{
	mutex_lock(&ldk_namespace_lock);
}

static void unlock_ldk_namespace(void)
{
	mutex_unlock(&ldk_namespace_lock);
}

static bool is_nsid_valid(int nsid)
{
	return  nsid >= 0 && nsid < MAX_NAMESPACE_NR;
}

int devhost_get_netns(struct net **netns)
{
	int err = 0;
	unsigned int nsid = ROOT_NET_NSID;

	err = devhost_get_net_nsid(&nsid);
	if (err != 0) {
		return err;
	}

	if (!is_nsid_valid(nsid)) {
		return -EINVAL;
	}

	/* init_net never to be destroyed, get without lock for fastpath */
	if (nsid == ROOT_NET_NSID) {
		*netns = get_net(&init_net);
		return 0;
	}

	lock_ldk_namespace();
	if (netns_arr[nsid] != NULL) {
		*netns = netns_arr[nsid];
		(void)get_net(*netns);
	} else {
		err = -EINVAL;
	}
	unlock_ldk_namespace();

	return err;
}

static int devhost_net_alloc_new_ns(int nsid)
{
	struct user_namespace *user_ns;
	struct net *new_net, *old_net;

	if (!is_nsid_valid(nsid)) {
		return -EINVAL;
	}

	old_net = (struct net *)kmalloc(sizeof(struct net), GFP_KERNEL);
	if (old_net == NULL) {
		return -ENOMEM;
	}

	old_net->nsid = (unsigned int)nsid;
	user_ns = &init_user_ns;
	new_net = copy_net_ns(CLONE_NEWNET, user_ns, old_net);
	kfree(old_net);
	if (IS_ERR(new_net)) {
		return -EINVAL;
	}

	lock_ldk_namespace();
	if (netns_arr[nsid] != NULL) {
		put_net(new_net);
		unlock_ldk_namespace();
		return -EEXIST;
	}
	netns_arr[nsid] = new_net;
	unlock_ldk_namespace();
	return 0;
}

static int devhost_net_destroy_ns(int nsid)
{
	if (!is_nsid_valid(nsid) || nsid == ROOT_NET_NSID) {
		return -EINVAL;
	}

	lock_ldk_namespace();
	if (netns_arr[nsid] == NULL) {
		unlock_ldk_namespace();
		return -EINVAL;
	}
	put_net(netns_arr[nsid]);
	netns_arr[nsid] = NULL;
	unlock_ldk_namespace();
	return 0;
}

static struct devhost_net_namespace_ops g_net_namespace_ops __read_mostly = {
	.alloc_new_ns = devhost_net_alloc_new_ns,
	.destroy_ns = devhost_net_destroy_ns,
};

#ifdef CONFIG_VENDOR_HOOKS
static void hook_devhost_net_socket_get_ns_by_fd(void *__data,int fd, struct net **net)
{
	int err;
	unsigned int nsid;

	err = devhost_get_nsid_by_fd(fd, &nsid);
	if (err != 0) {
		return;
	}

	if (!is_nsid_valid(nsid)) {
		return;
	}

	lock_ldk_namespace();
	if (netns_arr[nsid] != NULL) {
		*net = netns_arr[nsid];
		(void)get_net(*net);
	}
	unlock_ldk_namespace();

	return;
}

static void hook_devhost_net_socket_get_ns_by_pid(void *__data, int pid, struct net **net)
{
	int err;
	unsigned int nsid;

	err = devhost_get_nsid_by_pid(pid, CLONE_NEWNET, &nsid);
	if (err != 0) {
		return;
	}

	if (!is_nsid_valid(nsid)) {
		return;
	}

	lock_ldk_namespace();
	if (netns_arr[nsid] != NULL) {
		*net = netns_arr[nsid];
		(void)get_net(*net);
	}
	unlock_ldk_namespace();

	return;
}
#endif

static void hook_devhost_net_socket_get_netns(void *__data, struct net **net)
{
	int ret = devhost_get_netns(net);
	if (ret < 0) {
		*net = NULL;
		pr_err("failed to get netns, ret=%d\n", ret);
	}
}
INIT_VENDOR_HOOK(ldk_vh_devhost_net_socket_get_netns, hook_devhost_net_socket_get_netns);

void devhost_net_namespace_init(void)
{
#ifdef CONFIG_VENDOR_HOOKS
	int ret;
	ret = register_trace_ldk_vh_devhost_net_socket_get_ns_by_pid(hook_devhost_net_socket_get_ns_by_pid, NULL);
	ret |= register_trace_ldk_vh_devhost_net_socket_get_ns_by_fd(hook_devhost_net_socket_get_ns_by_fd, NULL);
	ret |= register_trace_ldk_vh_devhost_net_socket_get_netns(hook_devhost_net_socket_get_netns, NULL);
	if (ret != 0) {
		pr_warn("hooks, liblinux: register trace ldk hooks failed, ret=%d\n", ret);
	}
#endif

	/* register net namespace ops */
	devhost_net_namespace_register(&g_net_namespace_ops);

	netns_arr[ROOT_NS_ID] = &init_net;
	atomic64_set(&init_net.net_cookie, ROOT_NS_ID);
	(void)get_net(&init_net);

	return;
}
EXPORT_SYMBOL(devhost_net_namespace_init);
