/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header file for vlan
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 5 16:43:41 2021
 */
#ifndef _UX_IF_VLAN_H
#define _UX_IF_VLAN_H

#ifdef __cplusplus
extern "C" {
#endif

#define VLAN_VID_MASK	0x0fff /* The max VID value */

/* Determine which behaviour in vlan_ioctl_args structure */
#define ADD_VLAN_CMD 0
#define DEL_VLAN_CMD 1

#ifdef __cplusplus
}
#endif

#endif /* _UX_IF_VLAN_H */
