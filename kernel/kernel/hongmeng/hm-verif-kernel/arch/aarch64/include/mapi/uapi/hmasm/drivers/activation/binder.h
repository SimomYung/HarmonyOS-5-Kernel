/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: The binder IPC based on the Activation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 19 20:04:22 2023
 */

#ifndef AARCH64_UAPI_HMASM_DRIVERS_ACTIVATION_BINDER_H
#define AARCH64_UAPI_HMASM_DRIVERS_ACTIVATION_BINDER_H

/* Keep compatible with the ioctl command format */
#define __ACTV_BINDER_IOC(a, b, c, d)	(((a) << 30) | ((b) << 8) | (c) | ((d) << 16))

#define __ACTV_BINDER_IOC_NONE		0U
#define __ACTV_BINDER_IOC_WRITE		1U
#define __ACTV_BINDER_IOC_READ		2U

#define __ACTV_BINDER_IO(a, b)		__ACTV_BINDER_IOC(__ACTV_BINDER_IOC_NONE, (a), (b), 0)
#define __ACTV_BINDER_IOW(a, b, c)	__ACTV_BINDER_IOC(__ACTV_BINDER_IOC_WRITE, (a), (b), sizeof(c))
#define __ACTV_BINDER_IOR(a, b, c)	__ACTV_BINDER_IOC(__ACTV_BINDER_IOC_READ, (a), (b), sizeof(c))
#define __ACTV_BINDER_IOWR(a, b, c)	__ACTV_BINDER_IOC(__ACTV_BINDER_IOC_READ | \
							  __ACTV_BINDER_IOC_WRITE, (a), (b), sizeof(c))

#endif /* !AARCH64_UAPI_HMASM_DRIVERS_ACTIVATION_BINDER_H */
