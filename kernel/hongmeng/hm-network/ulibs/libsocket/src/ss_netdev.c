/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation for netdev
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 09:51:38 2019
 */
#include <errno.h>
#include <stdio.h>

#include <libhmlog/hmlog.h>
#include <libsysif_net/api.h>
#include <lib/utils.h>

#include "ss_channel.h"
/* maybe change it to hm_netdev.h in the future */
#include "hmnet_devices.h"

int hm_netdev_rx(unsigned int cell_pool_offset, int shm_len, int rx_len,
		  int offset, int devno)
{
	int ret;
	rref_t rref;

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_netdev_rx(rref,
						cell_pool_offset, shm_len,
						rx_len, offset, devno);

	return ret;
}

int hm_netdev_rx_ex(int num, void *data, int data_size)
{
	int ret;
	rref_t rref;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret =  actvcapcall_netcall_netdev_rx_ex_slowdata(rref,
								num,  data,
								(size_t)(unsigned int)data_size);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_netdev_notify(enum hm_netdev_event event, const char *ifname, int devno,
		     int option, uint64_t features)
{
	struct netdev_ifname devname = {0};
	rref_t rref;
	int ret;

	ret = safe_copy(devname.name, ifname, sizeof(devname.name));
	if (ret != 0) {
		return ret;
	}

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_netdev_notify(rref,
							event, devno,
							option, devname, features);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_netdev_update_features(int devno, uint64_t features)
{
	struct netdev_ifname devname = {0};
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_netdev_notify(rref,
							NETDEV_FEAT_CHANGE,
							devno,
							0, devname, features);

	hm_net_put_rref(&rref);
	return ret;
}

void hm_netdev_fifo_shm_init(uint32_t shm_size, char *fifo)
{
	ss_netdevfifo_t *pfifo = NULL;
	uint32_t fifo_shm_size;

	if (fifo == NULL) {
		return;
	}

	/* tx: 0-shm_size/2, rx: shm_size/2-shm_size */
	fifo_shm_size = (shm_size / 2);

	if (fifo_shm_size < sizeof(ss_netdevfifo_t)) {
		return;
	}

	bzero(fifo, (size_t)shm_size);

	pfifo = (ss_netdevfifo_t *)fifo;
	(void)raw_atomic_int_set(&pfifo->used, 0);
	(void)raw_sem_init(&pfifo->sem, 0);
	pfifo->pkt_maxcnt = (fifo_shm_size - (uint32_t)sizeof(ss_netdevfifo_t)) /
		(uint32_t)sizeof(struct netdev_pktinfo);

	pfifo = (ss_netdevfifo_t *)(void *)(fifo + fifo_shm_size);
	(void)raw_atomic_int_set(&pfifo->used, 0);
	(void)raw_sem_init(&pfifo->sem, 0);
	pfifo->pkt_maxcnt = (fifo_shm_size - (uint32_t)sizeof(ss_netdevfifo_t)) /
		(uint32_t)sizeof(struct netdev_pktinfo);

	hm_info("shm_size=%u, fifo_shm_size=%u, pkt_maxcnt=%u\n",
		shm_size, fifo_shm_size, pfifo->pkt_maxcnt);
}
