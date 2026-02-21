/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of fork 
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */
#include <errno.h>
#include <libsysif_net/api.h>

#include "hm_socket.h"
#include "ux_event.h"

int net_notify_fork(cref_t ch, uint32_t src_cidx, uint32_t dst_cidx)
{
	int ret;
	/* 32 is uint32_t bit */
	uint64_t cnode_idx = (((uint64_t)src_cidx << 32) | dst_cidx);
	unsigned long long rpc_info =
		__RPC_INFO_ENCODE(__hmnet_method_socket_fork, sizeof(uint64_t), 0);

	do {
		ret = syscap_ActivationPoolSendBufCall(ch, rpc_info, &cnode_idx);
	} while (ret == E_HM_ACTIVATION_RPC_RETRY);

	return ret;
}
