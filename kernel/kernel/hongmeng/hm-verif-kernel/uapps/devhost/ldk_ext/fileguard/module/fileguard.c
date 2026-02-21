/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define interface of fileguard
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 23 22:19:36 CST 2023
 */

#include <linux/file.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/printk.h>
#include "fileguard_api.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("fileguard ops");
MODULE_VERSION("0.01");

static int fileguard_message_notify(const void *message, unsigned int len)
{
	pr_info("[fg] fileguard message notify\n");
	return network_actvcall_message_handler(message, len);
}

static int __init devhost_fileguard_init(void)
{
	devhost_fileguard_ops_register(fileguard_message_notify);

	return 0;
}

module_init(devhost_fileguard_init);
