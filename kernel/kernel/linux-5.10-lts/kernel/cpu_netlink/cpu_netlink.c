/*
 * cpu_netlink.c
 *
 * iaware cpu netlink implementation
 *
 * Copyright (c) 2017-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <cpu_netlink/cpu_netlink.h>

#include <securec.h>
#if IS_ENABLED(CONFIG_COMMON_HW_RTG_FRAME)
#include <trace/hooks/sched.h>
#endif

#define MAX_DATA_LEN 20
#define NETLINK_HW_IAWARE_CPU 43

static __u32 recv_pid;
static struct sock *netlink_fd;
static DEFINE_MUTEX(netlink_lock);

int send_to_user(int sock_no, size_t len, const int *data)
{
	int ret = -1;
	int i;
	int num = len + SEND_DATA_LEN;
	int size;
	int dt[MAX_DATA_LEN];
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlh = NULL;

	if (!data) {
		printk(KERN_ERR "cpu_netlink: in send to user !data and return!\n");
		return ret;
	}

	mutex_lock(&netlink_lock);
	if (IS_ERR_OR_NULL(netlink_fd)) {
		printk(KERN_ERR "cpu_netlink: in send to user IS_ERR_OR_NULL netlink_fd and return!\n");
		goto out;
	}

	if (recv_pid <= 0) {
		printk(KERN_ERR "cpu_netlink: in send to user recv_pid <= 0 and return!\n");
		goto out;
	}

	if (num > MAX_DATA_LEN) {
		printk(KERN_ERR "cpu_netlink in send to user send oversize(%d,MAX:%d) data!\n", num, MAX_DATA_LEN);
		goto out;
	}

	dt[0] = sock_no;
	dt[1] = len;
	/*lint -save -e440 -e679*/
	for (i = 0; i + SEND_DATA_LEN < num; i++)
		dt[i + SEND_DATA_LEN] = data[i];

	/*lint -restore*/
	size = sizeof(int) * num;

	skb = alloc_skb(NLMSG_SPACE(size), GFP_ATOMIC);
	if (IS_ERR_OR_NULL(skb)) {
		printk(KERN_ERR "cpu_netlink %s: in send to user alloc skb failed!\n", __func__);
		ret = -ENOMEM;
		goto out;
	}
	nlh = nlmsg_put(skb, 0, 0, 0, NLMSG_SPACE(size) - sizeof(struct nlmsghdr), 0);
	ret = memcpy_s(NLMSG_DATA(nlh), size, (void *)dt, size);
	if (ret != EOK) {
		printk(KERN_ERR "cpu_netlink in send to user ret != EOK nad return!\n");
		goto out;
	}

	/* send up msg */
	ret = netlink_unicast(netlink_fd, skb, recv_pid, MSG_DONTWAIT);
	if (ret < 0)
		printk(KERN_ERR "cpu_netlink: in send to user netlink_unicast failed! errno = %d\n", ret);

out:
	mutex_unlock(&netlink_lock);
	return ret;
}

static void recv_from_user(struct sk_buff *skb)
{
	struct sk_buff *tmp_skb = NULL;
	struct nlmsghdr *nlh = NULL;

	mutex_lock(&netlink_lock);
	if (IS_ERR_OR_NULL(skb) || IS_ERR_OR_NULL(netlink_fd)) {
		printk(KERN_ERR "cpu_netlink: recv_from_user: sk_buff is NULL!\n");
		mutex_unlock(&netlink_lock);
		return;
	}

	tmp_skb = skb_get(skb);
	if (sock_net(netlink_fd) == sock_net(skb->sk)) {
		printk(KERN_ERR "cpu_netlink: netlink_fd == skb->sk\n");
	}
	mutex_unlock(&netlink_lock);
	if (tmp_skb->len >= NLMSG_SPACE(0)) {
		nlh = nlmsg_hdr(tmp_skb);
		recv_pid = nlh->nlmsg_pid;
		printk(KERN_ERR "cpu_netlink: in recv from user recv_pid =%d, netlink_fd = %p!\n", recv_pid, netlink_fd);
	}
}

static int __net_init netlink_net_init(struct net *net)
{
	struct netlink_kernel_cfg cfg = {
		.groups = 0,
		.input = recv_from_user,
	};
	mutex_lock(&netlink_lock);
	if (net == &init_net || !IS_ERR_OR_NULL(netlink_fd)) {
		mutex_unlock(&netlink_lock);
		return 0;
	}
	netlink_fd = netlink_kernel_create(net, NETLINK_HW_IAWARE_CPU, &cfg);
	if (IS_ERR_OR_NULL(netlink_fd)) {
		PTR_ERR(netlink_fd);
		printk(KERN_ERR "cpu_netlink: create cpu netlink error! ret is %d\n");
	}
	mutex_unlock(&netlink_lock);

	return 0;
}

static void __net_exit netlink_net_exit(struct net *net)
{
	mutex_lock(&netlink_lock);
	if (!IS_ERR_OR_NULL(netlink_fd) && !IS_ERR_OR_NULL(netlink_fd->sk_socket) && sock_net(netlink_fd) == net) {
		sock_release(netlink_fd->sk_socket);
		netlink_fd = NULL;
	}
	mutex_unlock(&netlink_lock);
}

static struct pernet_operations netlink_net_ops = {
	.init = netlink_net_init,
	.exit = netlink_net_exit,
};

static void destroy_cpu_netlink(void)
{
	mutex_lock(&netlink_lock);
	if (!IS_ERR_OR_NULL(netlink_fd) && !IS_ERR_OR_NULL(netlink_fd->sk_socket)) {
		sock_release(netlink_fd->sk_socket);
		netlink_fd = NULL;
	}
	mutex_unlock(&netlink_lock);
}

static int __init cpu_netlink_init(void)
{
	register_pernet_subsys(&netlink_net_ops);
	return 0;
}

static void __exit cpu_netlink_exit(void)
{
	destroy_cpu_netlink();
}

module_init(cpu_netlink_init);
module_exit(cpu_netlink_exit);
