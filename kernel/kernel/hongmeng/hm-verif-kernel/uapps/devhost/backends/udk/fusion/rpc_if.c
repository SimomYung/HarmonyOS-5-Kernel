/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: UDK sys rpc if
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 08 09:53:13 2024
 */

#include <libsysif/devhost/server.h>
#include <libsysif/fs/api.h>
#include <libhmsrv_io/blk.h>

#include "sd/include/api.h"

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_rpc_invoke,
		    unsigned long long, sender, unsigned long, credential,
		    int, devid, int, cmd,
		    unsigned long, req_ubuf, unsigned long, req_size,
		    unsigned long, resp_ubuf, unsigned long, resp_size)
{
	UNUSED(sender);
	UNUSED(credential);
	UNUSED(devid);
	return udk_pal_proxy_handler(cmd, (void *)req_ubuf, (size_t)req_size,
				     (void *)resp_ubuf, (size_t)resp_size);
}
