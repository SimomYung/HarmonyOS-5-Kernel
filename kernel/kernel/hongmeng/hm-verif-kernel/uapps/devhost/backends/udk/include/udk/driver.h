/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of driver in udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 12 19:00:09 2020
 */
#ifndef	__UDK_DRIVER_H__
#define	__UDK_DRIVER_H__

#include <devhost/device.h>

int udk_register_driver(const char *libname,
			const char *compat,
			size_t compat_len,
			struct hm_driver_ops *ops,
			void *ctx);
int udk_unregister_driver(const char *libname);

#endif /* __UDK_DRIVER_H__ */
