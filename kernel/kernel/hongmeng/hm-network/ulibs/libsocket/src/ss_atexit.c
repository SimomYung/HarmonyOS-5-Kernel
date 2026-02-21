/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Callback for process exit 
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */

#include <libsysif_net/api.h>

#include "hm_socket.h"

void net_revoke_process(cref_t rref, uint32_t cnode_idx)
{
	int ret;
	unsigned long long rpc_info;

	rpc_info = __RPC_INFO_ENCODE(__hmnet_method_socket_revoke, sizeof(uint32_t), 0);

	do {
		ret = syscap_ActivationPoolSendBufCall(rref, rpc_info, &cnode_idx);
	} while (ret == E_HM_ACTIVATION_RPC_RETRY);
}
