/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of pbuf operations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 12 19:11:36 2019
 */
#ifndef __UDK_PBUF_PRIV_H__
#define __UDK_PBUF_PRIV_H__

#include <udk/net/netdev.h>

#define HM_ETH_RESERVED 32

static inline void udk_pbuf_reserve_eth(struct pkt_buf *pbuf)
{
	udk_pbuf_update_offset(pbuf, HM_ETH_RESERVED);
}

void pbuf_start_xmit(struct pkt_buf *pbuf);
void udk_pbuf_set_attr(struct pkt_buf *pbuf,
		       unsigned int shm_offset,
		       unsigned long long paddr,
		       void *data,
		       struct udk_net_device *dev);
struct pkt_buf *udk_alloc_pbuf(struct udk_net_device *dev, void *data);

#endif /* __UDK_PBUF_PRIV_H__ */
