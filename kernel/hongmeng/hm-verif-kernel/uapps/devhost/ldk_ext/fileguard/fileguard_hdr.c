/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Define interface of fileguard
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 08 11:54:30 2024
 */
#include "fileguard_hdr.h"

#include <devhost/plugin.h>
#include <hongmeng/errno.h>
#include <libdevhost/devhost.h>
#include <libhmlog/hmlog.h>
#include <lnxbase/export.h>

#include "fileguard_api.h"

static struct devhost_file_guard_ops g_devhost_fileguard_ops __read_mostly;

PUBLIC_SYMBOL
void devhost_fileguard_ops_register(fileguard_actvcall_func actvcall_hdr)
{
	if (g_devhost_fileguard_ops.msg_handler == NULL) {
		g_devhost_fileguard_ops.msg_handler = actvcall_hdr;
		hm_info("[fg] fileguard ops registered\n");
	}
}
EXPORT_SYMBOL(devhost_fileguard_ops_register);

int devhost_fileguard_msg_notify(const void *message, unsigned int len)
{
	int ret = E_HM_OK;

	if ((g_devhost_fileguard_ops.msg_handler != NULL)) {
		hm_debug("[fg] fileguard msg notify\n");
		ret = g_devhost_fileguard_ops.msg_handler(message, len);
		ret = posix2hmerrno(-ret);
	}

	return ret;
}