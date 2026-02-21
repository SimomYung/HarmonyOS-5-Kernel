/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Functions for pbuf management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 16 10:45:12 2019
 */

#include "pbuf_priv.h"
#include <devhost/memory.h>
#include <udk/log.h>
#include <udk/slab.h>
#include <udk/sync.h>
#include <udk/iolib.h>
#include <udk/device.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>

/* to release pbuf and data for net layer */
void udk_free_pbuf(struct pkt_buf *pbuf)
{
	if (pbuf == NULL || pbuf->dev == NULL) {
		return;
	}
	pbuf->dev->dev_stats.devhost_free++;

	(void)devhost_dma_pool_free(pbuf->shm_offset);
	(void)udk_slab_unit_free(&pbuf->dev->pbuf_slab, pbuf);
}

/* to release pbuf and data for driver */
void udk_dev_free_pbuf(struct pkt_buf *pbuf)
{
	if (pbuf == NULL || pbuf->dev == NULL) {
		return;
	}
	pbuf->dev->dev_stats.drv_free++;

	udk_free_pbuf(pbuf);
}

void udk_pbuf_set_attr(struct pkt_buf *pbuf,
		       unsigned int shm_offset,
		       unsigned long long paddr,
		       void *data,
		       struct udk_net_device *dev)
{
	if (pbuf == NULL) {
		return;
	}
	pbuf->shm_offset = shm_offset;
	pbuf->unit_pa = paddr;
	pbuf->va = data;
	pbuf->dev = dev;
}

struct pkt_buf *udk_alloc_pbuf(struct udk_net_device *dev, void *data)
{
	unsigned int shm_offset = 0;
	unsigned long long paddr = 0;
	struct pkt_buf *pbuf = NULL;
	void *data_unit = NULL;
	int ret;

	if (dev == NULL) {
		udk_error("netdev is null\n");
		return NULL;
	}

	if (data == NULL) {
		data_unit = devhost_dma_pool_alloc(&paddr, &shm_offset);
		if (data_unit == NULL) {
			udk_error("devhost_dma_pool_alloc failed\n");
			return NULL;
		}
		dev->dev_stats.devhost_alloc++;
	} else {
		data_unit = data;
		shm_offset = pool_va2offset((uintptr_t)data);
		paddr = pool_offset2pa(shm_offset);
	}

	ret = udk_slab_unit_alloc(&dev->pbuf_slab, (void **)&pbuf);
	if (ret < 0) {
		udk_error("udk_slab_unit_alloc fail\n");
		(void)devhost_dma_pool_free(shm_offset);
		dev->dev_stats.devhost_free++;
		return NULL;
	}

	mem_zero_s(*pbuf);

	udk_pbuf_set_attr(pbuf, shm_offset, paddr, data_unit, dev);

	return pbuf;
}

struct pkt_buf *udk_dev_alloc_pbuf(struct udk_net_device *dev)
{
	struct pkt_buf *pbuf = NULL;

	if (dev == NULL) {
		return NULL;
	}
	pbuf = udk_alloc_pbuf(dev, NULL);
	if (pbuf != NULL) {
		udk_pbuf_reserve_eth(pbuf);
		dev->dev_stats.drv_alloc++;
	}

	return pbuf;
}

void pbuf_start_xmit(struct pkt_buf *pbuf)
{
	int ret;
	const struct udk_netdev_ops *ops = NULL;

	if ((pbuf == NULL) || (pbuf->dev == NULL) ||
	    (pbuf->dev->netdev_ops == NULL)) {
		return;
	}
	ops = pbuf->dev->netdev_ops;
	if (ops->start_xmit == NULL) {
		udk_error("start_xmit operation is null, register it first\n");
		return;
	}

	ret = ops->start_xmit(pbuf, pbuf->dev);
	if ((ret != (int)UDK_NETDEV_TX_OK) && (ret != (int)UDK_NETDEV_TX_BUSY)) {
		udk_error("unexpected return value, ret=%d\n", ret);
	}
}
