/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Arm gic v3
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 26 22:53:58 CST 2019
 */

#ifndef UAPI_HMKERNEL_GIC_V3_H
#define UAPI_HMKERNEL_GIC_V3_H

#define GICV3_IOCTL_HUAWEI_EXTERNAL_CORE_SGI		0x1

struct gicv3_extcore_ioctl_s {
	unsigned int logic_extcore_id;
};

#endif /* end of include guard GIC_V3_H */
