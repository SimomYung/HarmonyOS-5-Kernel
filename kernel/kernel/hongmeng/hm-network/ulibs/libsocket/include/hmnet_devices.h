/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for netdev
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 09:51:38 2019
 */

#ifndef __HMNET_DEVICES_H
#define __HMNET_DEVICES_H

#include <libhmsync/atomic.h>
#include <libhmsync/raw_sem.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HMNET_NETDEV_VERSION 2020022100

struct netdev_rx_cb {
	unsigned int cell_pool_offset;
	int rx_len;
	int offset;
	int dev_idx;
};
#define MAX_RX_CB_NUM 16

enum hm_netdev_event {
	NETDEV_ADD,
	NETDEV_REMOVE,
	NETDEV_LINKUP,
	NETDEV_LINKDOWN,
	NETDEV_FEAT_CHANGE,
};

int hm_netdev_rx(unsigned int cell_pool_offset, int shm_len, int rx_len,
		  int offset, int devno);
int hm_netdev_rx_ex(int num, void *data, int data_size);
int hm_netdev_notify(enum hm_netdev_event event, const char *ifname, int devno,
		     int option, uint64_t features);
int hm_netdev_update_features(int devno, uint64_t features);

static inline uint32_t netdev_fifo_next_head(uint32_t maxcnt, uint32_t head)
{
	return ((head == (maxcnt - 1U)) ? 0U : (head + 1U));
}

struct netdev_pktinfo {
	uint32_t pool_offset;
	uint32_t data_len;
	uint32_t data_offset;
	int linux_devno;
};

typedef struct ss_netdevfifo {
	struct raw_sem sem;
	uint32_t pkt_maxcnt;
	raw_atomic_int_t used;
	uint32_t c_head;
	uint32_t s_head;
	struct netdev_pktinfo pkt[];
} ss_netdevfifo_t;

void hm_netdev_fifo_shm_init(uint32_t shm_size, char *fifo);

#ifdef __cplusplus
}
#endif

#endif
