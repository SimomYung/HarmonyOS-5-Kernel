/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 14:17:56 2021
 */
#ifndef __DEVHOST_OF_PRIVATE_H__
#define __DEVHOST_OF_PRIVATE_H__

#include <linux/of.h>

/*
 * of live tree utilities from linux "drivers/of/of_private.h"
 */
struct property *__of_prop_dup(const struct property *prop, gfp_t allocflags);
__printf(2, 3) struct device_node *__of_node_dup(const struct device_node *np, const char *fmt, ...);

#endif /* __DEVHOST_OF_PRIVATE_H__ */
