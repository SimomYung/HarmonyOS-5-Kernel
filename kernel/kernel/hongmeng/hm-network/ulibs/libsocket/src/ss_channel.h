/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */

#ifndef __LIBSOCKET_SS_CHANNEL_H_
#define __LIBSOCKET_SS_CHANNEL_H_

#include <libsysif_net/api.h>
#include <libhmsrv_sys/hm_path.h>
#include "ux_event.h"

static inline int hm_net_ap_rref(rref_t* rref)
{
	return hm_path_acquire(HMNET_CHANNEL, rref);
}
int hm_net_ap_rref_and_cnode(rref_t* rref, uint32_t* cnode_idx);
static inline void hm_net_put_rref(const rref_t* rref)
{
	(void)hm_path_release(HMNET_CHANNEL, *rref);
}

int hm_net_ap_rref_noput(rref_t* rref);

#endif
