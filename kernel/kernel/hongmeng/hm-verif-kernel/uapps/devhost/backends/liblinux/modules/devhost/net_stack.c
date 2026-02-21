/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 16 17:52:40 2021
 */
#include <linux/if_arp.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>

#include "devhost.h"

static int devhost_packet_rcv(struct sk_buff *skb, struct net_device *dev,
			      struct packet_type *pt, struct net_device *orig_dev)
{
	int err = NET_RX_DROP;
	void *pool_va = NULL;
	unsigned int copy_size = 0;
	unsigned int offset = UINT_MAX;
	unsigned long long paddr = 0;

	/* restore ethernet header for hmnetd stack process */
	skb_push(skb, ETH_HLEN);
	copy_size = skb->len;
	pool_va = liblinux_pal_alloc_dma_pool_ex(&offset, &paddr, copy_size);
	if (pool_va == NULL) {
		goto err_out;
	}
	/* don't copy headroom */
	err = skb_copy_bits(skb, 0, pool_va, copy_size);
	if (err) {
		pr_err("copy whole skb content failed\n");
		goto err_out;
	}
	err = liblinux_pal_native_net_rx(offset, 0, copy_size, 0, dev->ifindex);

err_out:
	if (err) {
		if (offset != UINT_MAX) {
			liblinux_pal_free_dma_pool(offset);
		}
		err = NET_RX_DROP;
	} else {
		err = NET_RX_SUCCESS;
	}
	kfree_skb(skb);

	return err;
}

#define __packet_type_one(proto, recv)		\
	{					\
		.type = cpu_to_be16(proto),	\
		.func = (recv),			\
	}

#define dh_packet_type_entry(proto)		\
	__packet_type_one(proto, devhost_packet_rcv)

static struct packet_type packet_types[] = {
	dh_packet_type_entry(ETH_P_ARP),
	dh_packet_type_entry(ETH_P_IP),
	dh_packet_type_entry(ETH_P_CAN),
	dh_packet_type_entry(ETH_P_CANFD),
	dh_packet_type_entry(ETH_P_PAE),
	dh_packet_type_entry(ETH_P_PPP_MP),
};

int devhost_net_stack_init(void)
{
	unsigned int i;
	for (i = 0; i < ARRAY_SIZE(packet_types); ++i) {
		dev_add_pack(&packet_types[i]);
	}
	return 0;
}
